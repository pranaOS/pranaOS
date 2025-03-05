/**
 * @file tcpserver.cpp
 * @author Krisna Pranav
 * @brief tcp server
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/ipv4address.h>
#include <mods/types.h>
#include <libcore/notifier.h>
#include <libcore/system.h>
#include <libcore/tcpserver.h>

namespace Core 
{

    /**
     * @param parent 
     * @return ErrorOr<NonnullRefPtr<TCPServer>> 
     */
    ErrorOr<NonnullRefPtr<TCPServer>> TCPServer::try_create(Object* parent)
    {
    #ifdef SOCK_NONBLOCK
        int fd = TRY(Core::System::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0));
    #else
        int fd = TRY(Core::System::socket(AF_INET, SOCK_STREAM, 0));
        int option = 1;
        TRY(Core::System::ioctl(fd, FIONBIO, &option));
        TRY(Core::System::fcntl(fd, F_SETFD, FD_CLOEXEC));
    #endif

        return adopt_nonnull_ref_or_enomem(new (nothrow) TCPServer(fd, parent));
    }

    /**
     * @brief Construct a new TCPServer::TCPServer object
     * 
     * @param fd 
     * @param parent 
     */
    TCPServer::TCPServer(int fd, Object* parent)
        : Object(parent)
        , m_fd(fd)
    {
        VERIFY(m_fd >= 0);
    }

    /**
     * @brief Destroy the TCPServer::TCPServer object
     * 
     */
    TCPServer::~TCPServer()
    {
        MUST(Core::System::close(m_fd));
    }

    /**
     * @param address 
     * @param port 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> TCPServer::listen(IPv4Address const& address, u16 port)
    {
        if (m_listening)
            return Error::from_errno(EADDRINUSE);

        auto socket_address = SocketAddress(address, port);
        auto in = socket_address.to_sockaddr_in();

        TRY(Core::System::bind(m_fd, (sockaddr const*)&in, sizeof(in)));
        TRY(Core::System::listen(m_fd, 5));

        m_listening = true;

        m_notifier = Notifier::construct(m_fd, Notifier::Event::Read, this);
        m_notifier->on_ready_to_read = [this] {
            if (on_ready_to_accept)
                on_ready_to_accept();
        };
        return {};
    }

    /**
     * @param blocking 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> TCPServer::set_blocking(bool blocking)
    {
        int flags = TRY(Core::System::fcntl(m_fd, F_GETFL, 0));

        if (blocking)
            TRY(Core::System::fcntl(m_fd, F_SETFL, flags & ~O_NONBLOCK));
        else
            TRY(Core::System::fcntl(m_fd, F_SETFL, flags | O_NONBLOCK));

        return {};
    }

    /**
     * @return ErrorOr<NonnullOwnPtr<Stream::TCPSocket>> 
     */
    ErrorOr<NonnullOwnPtr<Stream::TCPSocket>> TCPServer::accept()
    {
        VERIFY(m_listening);
        sockaddr_in in;
        socklen_t in_size = sizeof(in);
    #ifndef MODS_OS_MACOS
        int accepted_fd = TRY(Core::System::accept4(m_fd, (sockaddr*)&in, &in_size, SOCK_NONBLOCK | SOCK_CLOEXEC));
    #else
        int accepted_fd = TRY(Core::System::accept(m_fd, (sockaddr*)&in, &in_size));
    #endif

        auto socket = TRY(Stream::TCPSocket::adopt_fd(accepted_fd));

    #ifdef MODS_OS_MACOS
        TRY(socket->set_blocking(false));
        TRY(socket->set_close_on_exec(true));
    #endif

        return socket;
    }

    /**
     * @return Optional<IPv4Address> 
     */
    Optional<IPv4Address> TCPServer::local_address() const
    {
        if (m_fd == -1)
            return {};

        sockaddr_in address;
        socklen_t len = sizeof(address);

        if (getsockname(m_fd, (sockaddr*)&address, &len) != 0)
            return {};

        return IPv4Address(address.sin_addr.s_addr);
    }

    /**
     * @return Optional<u16> 
     */
    Optional<u16> TCPServer::local_port() const
    {
        if (m_fd == -1)
            return {};

        sockaddr_in address;
        socklen_t len = sizeof(address);

        if (getsockname(m_fd, (sockaddr*)&address, &len) != 0)
            return {};

        return ntohs(address.sin_port);
    }

} // namespace Core
