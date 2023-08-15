/**
 * @file network_adapter.h
 * @author Krisna Pranav aka (krishpranav)
 * @brief network adapter
 * @version 1.0
 * @date 2023-08-15
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/userorkernelbuffer.h>
#include <kernel/kbuffer.h>
#include <kernel/net/ipv4.h>
#include <kernel/net/icmp.h>
#include <kernel/net/ipv4.h>
#include <mods/byte_buffer.h>
#include <mods/function.h>
#include <mods/mac_address.h>
#include <mods/singlelinkedlist.h>
#include <mods/types.h>
#include <mods/weakable.h>
#include <mods/weakptr.h>

namespace Kernel
{
    class NetworkAdapter;

    class NetworkAdapter : public RefCounted<NetworkAdapter>
    {

    private:
        MACAddress m_mac_address;
        IPv4Address m_ipv4_address;
        IPv4Address m_ipv4_netmask;
        IPv4Address m_ipv4_gateway;

        struct PacketWithTimestamp
        {
            KBuffer packet;
            timeval timestamp;
        }; // struct

        SinglyLinkedList<PacketWithTimestamp> m_packet_queue;
        SinglyLinkedList<KBuffer> m_unused_packet_buffer;
        size_t m_unused_packet_buffers_count { 0 };

        String m_name;

        u32 m_packets_in { 0 };
        u32 m_bytes_in { 0 };
        u32 m_packets_out { 0 };
        u32 m_bytes_out { 0 };
        u32 m_mtu { 1500 };

    }; // class NetworkAdapter

} // namespace Kernel