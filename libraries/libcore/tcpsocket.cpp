/**
 * @file tcpsocket.cpp
 * @author Krisna Pranav
 * @brief tcp socket
 * @version 6.0
 * @date 2023-10-02
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
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
    /**
     * @brief Construct a new TCPSocket::TCPSocket object
     * 
     * @param fd 
     * @param parent 
     */
    TCPSocket::TCPSocket(int fd, Object* parent)
        : Socket(Socket::Type::TCP, parent)
    {
        m_connected = true;
        set_fd(fd);
        set_mode(IODevice::ReadWrite);
        set_error(0);
    }

    /**
     * @brief Construct a new TCPSocket::TCPSocket object
     * 
     * @param parent 
     */
    TCPSocket::TCPSocket(Object* parent)
        : Socket(Socket::Type::TCP, parent)
    {
    #ifdef SOCK_NONBLOCK
        int fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    #else
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        int option = 1;
        ioctl(fd, FIONBIO, &option);
    #endif
        if (fd < 0) {
            set_error(errno);
        } else {
            set_fd(fd);
            set_mode(IODevice::ReadWrite);
            set_error(0);
        }
    }

    /// @brief Destroy the TCPSocket::TCPSocket object
    TCPSocket::~TCPSocket()
    { }

} // namesapce Core