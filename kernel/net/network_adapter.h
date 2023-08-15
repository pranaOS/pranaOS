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
    public:
        /// @breif: for_each
        static void for_each(Function<void(NetworkAdapter&)>);

        /**
         * @return RefPtr<NetworkAdapter> 
         */
        static RefPtr<NetworkAdapter> from_ipv4_address(const IPv4Address&);
        static RefPtr<NetworkAdapter> lookup_by_name(const StringView&);

        /// @brief Destroy the NetworkAdapter object
        virtual ~NetworkAdapter();

        /**
         * @return const char* 
         */
        virtual const char* class_name() const = 0;

        /**
         * @return const String& 
         */
        const String& name() const 
        { 
            return m_name; 
        }

        /**
         * @return MACAddress 
         */
        MACAddress mac_address() 
        { 
            return m_mac_address; 
        }

        /**
         * @return IPv4Address 
         */
        IPv4Address ipv4_address() const 
        { 
            return m_ipv4_address; 
        }
        
        IPv4Address ipv4_netmask() const 
        { 
            return m_ipv4_netmask; 
        }
        
        IPv4Address ipv4_gateway() const 
        { 
            return m_ipv4_gateway; 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool link_up() { return false; }

        /**
         * @brief Set the ipv4 address, netmask, gateway object
         * 
         */
        void set_ipv4_address(const IPv4Address&);
        void set_ipv4_netmask(const IPv4Address&);
        void set_ipv4_gateway(const IPv4Address&);

        /// @brief: send
        void send(const MACAddress&, const ARPPacket&);

        /**
         * @param payload 
         * @param payload_size 
         * @param ttl 
         * @return int 
         */
        int send_ipv4(const MACAddress&, const IPv4Address&, IPv4Protocol, const UserOrKernelBuffer& payload, size_t payload_size, u8 ttl);

        /**
         * @param payload 
         * @param payload_size 
         * @param ttl 
         * @return int 
         */
        int send_ipv4_fragmented(const MACAddress&, const IPv4Address&, IPv4Protocol, const UserOrKernelBuffer& payload, size_t payload_size, u8 ttl);

        /**
         * @param buffer 
         * @param buffer_size 
         * @param packet_timestamp 
         * @return size_t 
         */
        size_t dequeue_packet(u8* buffer, size_t buffer_size, timeval& packet_timestamp);

        /**
         * @return true 
         * @return false 
         */
        bool has_queued_packets() const 
        { 
            return !m_packet_queue.is_empty(); 
        }

        /**
         * @return u32 
         */
        u32 mtu() const 
        { 
            return m_mtu; 
        }

        /**
         * @param mtu 
         */
        void set_mtu(u32 mtu) { m_mtu = mtu; }

        /**
         * @return u32 
         */
        u32 packets_in() const 
        { 
            return m_packets_in; 
        }
        
        u32 bytes_in() const 
        { 
            return m_bytes_in; 
        }

        u32 packets_out() const 
        { 
            return m_packets_out; 
        }

        u32 bytes_out() const 
        { 
            return m_bytes_out; 
        }

        Function<void()> on_receive;

    protected:

        /// @brief Construct a new NetworkAdapter object
        NetworkAdapter();

        /**
         * @param basename 
         */
        void set_interface_name(const StringView& basename);

        /**
         * @param mac_address 
         */
        void set_mac_address(const MACAddress& mac_address) 
        { 
            m_mac_address = mac_address; 
        }

        /// @brief: send_raw bytes = 0;
        virtual void send_raw(ReadonlyBytes) = 0;

        /// @brief: did_receive? bytes.
        void did_receive(ReadonlyBytes);

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
        SinglyLinkedList<KBuffer> m_unused_packet_buffers;
        
        size_t m_unused_packet_buffers_count { 0 };

        String m_name;

        u32 m_packets_in { 0 };
        u32 m_bytes_in { 0 };
        u32 m_packets_out { 0 };
        u32 m_bytes_out { 0 };
        u32 m_mtu { 1500 };

    }; // class NetworkAdapter

} // namespace Kernel
