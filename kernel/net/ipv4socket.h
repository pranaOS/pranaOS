/**
 * @file ipv4socket.h
 * @author Krisna Pranav
 * @brief ipv4 socket
 * @version 1.0
 * @date 2023-08-16
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/hashmap.h>
#include <mods/singlelinkedlist.h>
#include <kernel/doublebuffer.h>
#include <kernel/kbuffer.h>
#include <kernel/lock.h>
#include <kernel/net/ipv4.h>
#include <kernel/net/ipv4socket_tuple.h>
#include <kernel/net/socket.h>

namespace Kernel
{
    class NetworkAdapter;
    class TCPPacket;
    class TCPSocket;

    class IPV4Socket : public Socket 
    {
    private:

        virtual bool is_ipv4() const override
        {
            return true;
        }
        
        IPv4Address m_local_address;
        IPv4Address m_peer_address;

        void set_can_read(bool);

        struct ReceivedPacket
        {
            IPv4Address peer_address;
            u16 peer_port;
            timeval timestamp;
            Optional<KBuffer> data;
        };

        SinglyLinkedListWithCount<ReceivedPacket> m_received_queue;

        bool m_can_read { false };

        BufferMode m_buffer_mode { BufferMode::Packets };
    }; // class IPV4Socket 

} // namespace Kernel