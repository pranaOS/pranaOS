/**
 * @file arp.h
 * @author Krisna Pranav aka (krishpranav)
 * @brief arp packets
 * @version 1.0
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
        };
    };
    
    struct ARPHardwareType
    {
        enum : u16
        {
            Ethernet = 1,
        };
    }; 

    class ARPPacket
    {
    public:
        /**
         * @return u16 
         */
        u16 hardware_type()
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

} // namespace Kernel