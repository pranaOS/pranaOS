/**
 * @file tcpsocket.h
 * @author Krisna Pranav
 * @brief tcp socket 
 * @version 6.0
 * @date 2023-10-02
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/badge.h>
#include <libcore/socket.h>

namespace Core
{

    class TCPSocket final : public Socket
    {
    public:
        ~TCPSocket() {}

        TCPSocket::~TCPSocket() {}

    private:
        /**
         * @param fd 
         * @param parent 
         */
        TCPSocket(int fd, Object* parent = nullptr);

        /**
         * @param parent 
         */
        explicit TCPSocket(Object* parent = nullptr);
    }; // class TCPSOCKET

} // namespace Core