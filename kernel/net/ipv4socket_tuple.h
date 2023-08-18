/**
 * @file ipv4socket_tuple.h
 * @author Krisna Pranav
 * @brief ipv4 socket tuple
 * @version 6.0
 * @date 2023-08-16
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/hashmap.h>
#include <kernel/doublebuffer.h>
#include <kernel/kbuffer.h>
#include <kernel/lock.h>
#include <kernel/net/ipv4.h>
#include <kernel/net/socket.h>

class IPv4SocketTuple 
{
public:
    /**
     * @param local_address 
     * @param local_port 
     * @param peer_address 
     * @param peer_port 
     */
    IPv4SocketTuple(IPv4Address local_address, u16 local_port, IPv4Address peer_address, u16 peer_port)
        : m_local_address(local_address)
        , m_local_port(local_port)
        , m_peer_address(peer_address)
        , m_peer_port(peer_port) {};

    /**
     * @return IPv4Address 
     */
    IPv4Address local_address() const 
    { 
        return m_local_address; 
    };

    /**
     * @return u16 
     */
    u16 local_port() const 
    { 
        return m_local_port; 
    };

    /**
     * @return IPv4Address 
     */
    IPv4Address peer_address() const 
    { 
        return m_peer_address; 
    };

    /**
     * @return u16 
     */
    u16 peer_port() const 
    { 
        return m_peer_port; 
    };
    
    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool operator==(const IPv4SocketTuple other) const
    {
        return other.local_address() == m_local_address && other.local_port() == m_local_port && other.peer_address() == m_peer_address && other.peer_port() == m_peer_port;
    };
    
    /**
     * @return String 
     */
    String to_string() const
    {
        return String::format(
            "%s:%d -> %s:%d",
            m_local_address.to_string().characters(),
            m_local_port,
            m_peer_address.to_string().characters(),
            m_peer_port);
    }

private:
    IPv4Address m_local_address;
    u16 m_local_port { 0 };
    IPv4Address m_peer_address;
    u16 m_peer_port { 0 };
}; // class IPv4SocketTuple

namespace Mods 
{
    template<>
    struct Traits<IPv4SocketTuple> : public GenericTraits<IPv4SocketTuple> {
        static unsigned hash(const IPv4SocketTuple& tuple)
        {
            auto h1 = pair_int_hash(tuple.local_address().to_u32(), tuple.local_port());
            auto h2 = pair_int_hash(tuple.peer_address().to_u32(), tuple.peer_port());
            return pair_int_hash(h1, h2);
        }
    }; // struct

} // namespace Mods
