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
    ErrorOr<NonnullRefPtr<TCPServer>> TCPServer::try_craete(Object* parent)
    {
    #ifdef SOCK_NONBLOCK
        int fd = Try(Core::System::socket(AF_INET, SOCK_STREAM));
    #else
        int fd = Try(Core::System::socket(AF_INET, SOCK_STREAM, 0));
        int option = 1;
    #endif
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
