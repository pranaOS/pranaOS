/**
 * @file udpsocket.h
 * @author Krisna Pranav
 * @brief udpsocket
 * @version 6.0
 * @date 2023-10-03
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <libcore/socket.h>

namespace Core
{

    class UDPSocket final : public Socket
    {
    public:
        ~UDPSocket();
        
    private:

        /**
         * @brief Construct a new UDPSocket object
         * 
         * @param parent 
         */
        explicit UDPSocket(Object* parent = nullptr);
    }; // clas UDPSocket

} // namesapce Core