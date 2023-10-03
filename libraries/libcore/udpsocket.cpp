/**
 * @file udpsocket.cpp
 * @author Krisna Pranav
 * @brief udpsocket
 * @version 6.0
 * @date 2023-10-03
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <errno.h>
#include <libcore/udpsocket.h>
#include <sys/socket.h>

#ifndef SOCK_NONBLOCK
#   include <sys/ioctl.h>
#endif

namespace Core
{
    /**
     * @param parent 
     */
    UDPSocket::UDPSocket(Object* parent)
        : Socket(Socket::Type::UDP, parent)
    {
        int fd = socket(AF_INET, SOCK_DGRAM, 0);
        int option = 1;
        ioctl(fd, FIONBIO, &option);

        if (fd < 0) {
            set_error(errno);
        } else {
            set_fd(fd);
            set_error(0);
        }
    }

    /// @brief Destroy the UDPSocket::UDPSocket object
    UDPSocket::~UDPSocket()
    {}
} // namespace Core