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
    class SocketAddress {
    public:
        enum class Type {
            Invalid,
            IPv4,
            Local
        };

        SocketAddress() { }
        SocketAddress(const IPv4Address& address)
            : m_type(Type::IPv4)
            , m_ipv4_address(address)
        {
        }

        SocketAddress(const IPv4Address& address, u16 port)
            : m_type(Type::IPv4)
            , m_ipv4_address(address)
            , m_port(port)
        {
        }

        static SocketAddress local(const String& address)
        {
            SocketAddress addr;
            addr.m_type = Type::Local;
            addr.m_local_address = address;
            return addr;
        }

        /**
         * @return Type 
         */
        Type type() const 
        { 
            return m_type; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_valid() const 
        { 
            return m_type != Type::Invalid; 
        }

        /**
         * @return IPv4Address 
         */
        IPv4Address ipv4_address() const 
        { 
            return m_ipv4_address; 
        }

        /**
         * @return u16 
         */
        u16 port() const 
        { 
            return m_port; 
        }

        /**
         * @return String 
         */
        String to_string() const
        {
            switch (m_type) {
            case Type::IPv4:
                return String::format("%s:%d", m_ipv4_address.to_string().characters(), m_port);
            case Type::Local:
                return m_local_address;
            default:
                return "[SocketAddress]";
            }
        }

        Optional<sockaddr_un> to_sockaddr_un() const
        {
            ASSERT(type() == Type::Local);
            sockaddr_un address;
            address.sun_family = AF_LOCAL;
            bool fits = m_local_address.copy_characters_to_buffer(address.sun_path, sizeof(address.sun_path));
            if (!fits)
                return {};
            return address;
        }

        /**
         * @return sockaddr_in 
         */
        sockaddr_in to_sockaddr_in() const
        {
            ASSERT(type() == Type::IPv4);
            sockaddr_in address {};
            address.sin_family = AF_INET;
            address.sin_addr.s_addr = m_ipv4_address.to_in_addr_t();
            address.sin_port = htons(m_port);
            return address;
        }

    private:
        Type m_type { Type::Invalid };
        IPv4Address m_ipv4_address;
        u16 m_port { 0 };
        String m_local_address;
    }; // class SocketAddress 

    /**
     * @return const LogStream& 
     */
    const LogStream& operator<<(const LogStream&, const SocketAddress&);
} // namespace Core