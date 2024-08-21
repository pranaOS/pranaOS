/**
 * @file tcpsocket.h
 * @author Krisna Pranav
 * @brief tcp socket 
 * @version 6.0
 * @date 2023-10-02
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/badge.h>
#include <libcore/socket.h>

namespace Core 
{
    class TCPSocket final : public Socket 
    {
        C_OBJECT(TCPSocket)

    public:
        /// @brief Destroy the TCPSocket object
        virtual ~TCPSocket() override;

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
    }; // class TCPSocket
} // namespace Core