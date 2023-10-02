/**
 * @file tcpsocket.cpp
 * @author Krisna Pranav
 * @brief tcp socket
 * @version 6.0
 * @date 2023-10-02
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <errno.h>
#include <libcore/tcpsocket.h>
#include <sys/socket.h>

#ifndef SOCK_NONBLOCK
#   include <sys/ioctl.h>
#endif

namespace Core
{

    TCPSocket::TCPSocket(int fd, Object* parent)
            : Socket(Socket::Type::TCP, parent)
    {
        m_connected = true;
        set_fd(fd);
    }

    TCPSocket::~TCPSocket()
    {}

} // namespace Core