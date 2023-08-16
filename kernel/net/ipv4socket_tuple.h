/**
 * @file ipv4socket_tuple.h
 * @author Krisna Pranav
 * @brief ipv4 socket tuple
 * @version 0.1
 * @date 2023-08-16
 * 
 * @copyright Copyright (c) 2023
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
        , m_peer_port(peer_port) {}


    u16 local_peer() const
    {
        return m_local_port;
    }

    u16 peer_port() const
    {
        return m_peer_port;
    }

private:
    IPv4Address m_local_address;
    u16 m_local_port { 0 };
    IPv4Address m_peer_address;
    u16 m_peer_port;
}; // class IPv4SocketTuple;