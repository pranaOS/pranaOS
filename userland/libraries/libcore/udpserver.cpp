/**
 * @file udpserver.cpp
 * @author Krisna Pranav
 * @brief udp server
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/ipv4address.h>
#include <mods/types.h>
#include <libcore/notifier.h>
#include <libcore/udpserver.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#ifndef SOCK_NONBLOCK
#    include <fcntl.h>
#    include <sys/ioctl.h>
#endif

namespace Core 
{

    /**
     * @brief Construct a new UDPServer::UDPServer object
     * 
     * @param parent 
     */
    UDPServer::UDPServer(Object* parent)
        : Object(parent)
    {
    #ifdef SOCK_NONBLOCK
        m_fd = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
    #else
        m_fd = socket(AF_INET, SOCK_DGRAM, 0);
        int option = 1;
        ioctl(m_fd, FIONBIO, &option);
        fcntl(m_fd, F_SETFD, FD_CLOEXEC);
    #endif
        VERIFY(m_fd >= 0);
    }

    /**
     * @brief Destroy the UDPServer::UDPServer object
     * 
     */
    UDPServer::~UDPServer()
    {
        ::close(m_fd);
    }

    /**
     * @param address 
     * @param port 
     * @return true 
     * @return false 
     */
    bool UDPServer::bind(IPv4Address const& address, u16 port)
    {
        if (m_bound)
            return false;

        auto saddr = SocketAddress(address, port);
        auto in = saddr.to_sockaddr_in();

        if (::bind(m_fd, (sockaddr const*)&in, sizeof(in)) != 0) {
            perror("UDPServer::bind");
            return false;
        }

        m_bound = true;

        m_notifier = Notifier::construct(m_fd, Notifier::Event::Read, this);
        m_notifier->on_ready_to_read = [this] {
            if (on_ready_to_receive)
                on_ready_to_receive();
        };
        return true;
    }

    /**
     * @param size 
     * @param in 
     * @return ByteBuffer 
     */
    ByteBuffer UDPServer::receive(size_t size, sockaddr_in& in)
    {
        auto buf = ByteBuffer::create_uninitialized(size).release_value_but_fixme_should_propagate_errors();
        socklen_t in_len = sizeof(in);
        ssize_t rlen = ::recvfrom(m_fd, buf.data(), size, 0, (sockaddr*)&in, &in_len);

        if (rlen < 0) {
            dbgln("recvfrom: {}", strerror(errno));
            return {};
        }

        buf.resize(rlen);
        return buf;
    }

    /**
     * @return Optional<IPv4Address> 
     */
    Optional<IPv4Address> UDPServer::local_address() const
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
    Optional<u16> UDPServer::local_port() const
    {
        if (m_fd == -1)
            return {};

        sockaddr_in address;
        socklen_t len = sizeof(address);
        if (getsockname(m_fd, (sockaddr*)&address, &len) != 0)
            return {};

        return ntohs(address.sin_port);
    }

    /**
     * @param buffer 
     * @param to 
     * @return ErrorOr<size_t> 
     */
    ErrorOr<size_t> UDPServer::send(ReadonlyBytes buffer, sockaddr_in const& to)
    {
        if (m_fd < 0) {
            return Error::from_errno(EBADF);
        }

        auto result = ::sendto(m_fd, buffer.data(), buffer.size(), 0, (sockaddr const*)&to, sizeof(to));
        if (result < 0) {
            return Error::from_errno(errno);
        }

        return result;
    }

} // namespace Core
