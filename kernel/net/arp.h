/**
 * @file arp.h
 * @author Krisna Pranav aka (krishpranav)
 * @brief arp packets
 * @version 6.0
 * @date 2023-08-15
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/endian.h>
#include <mods/mac_address.h>
#include <kernel/net/ethertype.h>
#include <kernel/net/ipv4.h>

namespace Kernel 
{

    struct ARPOperation 
    {
        enum : u16 
        {
            Request = 1,
            Response = 2,
        }; // enum
    }; // struct 

    struct ARPHardwareType 
    {
        enum : u16 
        {
            Ethernet = 1,
        }; // enum
    }; // struct

    class [[gnu::packed]] ARPPacket
    {
    public:

        /**

         * @return u16 
         */
        u16 hardware_type() const 
        { 
            return m_hardware_type; 
        }

        /**
         * @param w 
         */
        void set_hardware_type(u16 w) 
        { 
            m_hardware_type = w; 
        }

        /**
         * @return u16 
         */
        u16 protocol_type() const 
        { 
            return m_protocol_type; 
        }

        /**
         * @param w 
         */
        void set_protocol_type(u16 w) 
        { 
            m_protocol_type = w; 
        }

        /**
         * @return u8 
         */
        u8 hardware_address_length() const 
        { 
            return m_hardware_address_length; 
        }

        /**
         * @param b 
         */
        void set_hardware_address_length(u8 b) 
        { 
            m_hardware_address_length = b; 
        }

        /**
         * @return u8 
         */
        u8 protocol_address_length() const 
        { 
            return m_protocol_address_length; 
        }

        /**
         * @param b 
         */
        void set_protocol_address_length(u8 b) 
        { 
            m_protocol_address_length = b; 
        }

        /**
         * @return u16 
         */
        u16 operation() const 
        { 
            return m_operation; 
        }

        /**
         * @param w 
         */
        void set_operation(u16 w) 
        { 
            m_operation = w; 
        }

        /**
         * @return const MACAddress& 
         */
        const MACAddress& sender_hardware_address() const 
        { 
            return m_sender_hardware_address; 
        }

        /**
         * @param address 
         */
        void set_sender_hardware_address(const MACAddress& address) 
        { 
            m_sender_hardware_address = address; 
        }

        /**
         * @return const IPv4Address& 
         */
        const IPv4Address& sender_protocol_address() const 
        { 
            return m_sender_protocol_address; 
        }

        /**
         * @param address 
         */
        void set_sender_protocol_address(const IPv4Address& address) 
        { 
            m_sender_protocol_address = address; 
        }

        /**
         * @return const MACAddress& 
         */
        const MACAddress& target_hardware_address() const 
        { 
            return m_target_hardware_address; 
        }

        /**
         * @param address 
         */
        void set_target_hardware_address(const MACAddress& address) 
        { 
            m_target_hardware_address = address; 
        }

        /**
         * @return const IPv4Address& 
         */
        const IPv4Address& target_protocol_address() const 
        { 
            return m_target_protocol_address; 
        }

        /**
         * @param address 
         */
        void set_target_protocol_address(const IPv4Address& address) 
        { 
            m_target_protocol_address = address; 
        }

    private:

        NetworkOrdered<u16> m_hardware_type { ARPHardwareType::Ethernet };
        NetworkOrdered<u16> m_protocol_type { EtherType::IPv4 };
        
        u8 m_hardware_address_length { sizeof(MACAddress) };
        u8 m_protocol_address_length { sizeof(IPv4Address) };

        NetworkOrdered<u16> m_operation;

        MACAddress m_sender_hardware_address;
        IPv4Address m_sender_protocol_address;
        MACAddress m_target_hardware_address;
        IPv4Address m_target_protocol_address;
        
    }; // class ARPPacket

    static_assert(sizeof(ARPPacket) == 28);

} // namespace Kernel
