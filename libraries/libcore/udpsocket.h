/**
 * @file udpsocket.h
 * @author Krisna Pranav
 * @brief udpsocket
 * @version 6.0
 * @date 2023-10-03
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <libcore/socket.h>

namespace Core 
{
    class UDPSocket final : public Socket 
    {
        C_OBJECT(UDPSocket)

    public:
        /// @brief Destroy the UDPSocket object
        virtual ~UDPSocket() override;

    private:

        /**
         * @brief Construct a new UDPSocket object
         * 
         * @param parent 
         */
        explicit UDPSocket(Object* parent = nullptr);
    }; // class UDPSocket
} // namesapce Core