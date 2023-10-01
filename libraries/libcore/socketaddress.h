/**
 * @file socketaddress.h
 * @author Krisna Pranav
 * @brief socket address
 * @version 6.0
 * @date 2023-10-01
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/ipv4address.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

namespace Core
{

    class SocketAddress
    {   
    public:
        enum class Type
        {
            Invalid,
            IPv4,
            Local
        }; // enum class Type

        SocketAddress() {}

        SocketAddress(const IPv4Address& address)
            : m_type(Type::IPv4)
            , m_ipv4_address(address)
        {}

        static SocketAddress local(const String& address)
        {
            SocketAddress addr;
            addr.m_type = Type::Local;
            addr.m_local_address = address;
            return addr;
        }

        Type type() const
        {
            return m_type;
        }

        bool is_valid() const
        {
            return m_type != Type::Invalid;
        }

        IPv4Address ipv4_address() const
        {
            return m_ipv4_address;
        }

    private:
        Type m_type { Type::Invalid };
        IPv4Address m_ipv4_address;
        u16 m_port { 0 };
        String m_local_address;
    }; // class SocketAddress 
        
} // namespace Core