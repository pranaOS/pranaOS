/**
 * @file ethernetframeheader.h
 * @author Krisna Pranav
 * @brief ethernet frame header
 * @version 6.0
 * @date 2023-08-17
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/endian.h>
#include <mods/mac_address.h>

#pragma GCC diagnostic ignored "-Warray-bounds"

class [[gnu::packed]] EthernetFrameHeader
{
public:
    EthernetFrameHeader() { }
    ~EthernetFrameHeader() { }

    /**
     * @return MACAddress 
     */
    MACAddress destination() const 
    { 
        return m_destination; 
    }

    /**
     * @param address 
     */
    void set_destination(const MACAddress& address) 
    { 
        m_destination = address; 
    }

    /**
     * @return MACAddress 
     */
    MACAddress source() const 
    { 
        return m_source; 
    }

    /**
     * @param address 
     */
    void set_source(const MACAddress& address) 
    { 
        m_source = address; 
    }

    /**
     * @return u16 
     */
    u16 ether_type() const 
    { 
        return m_ether_type; 
    }

    /**
     * @param ether_type 
     */
    void set_ether_type(u16 ether_type) 
    { 
        m_ether_type = ether_type; 
    }
    
    /**
     * @return const void* 
     */
    const void* payload() const 
    { 
        return &m_payload[0]; 
    }

    /**
     * @return void* 
     */
    void* payload() 
    { 
        return &m_payload[0]; 
    }

private:
    MACAddress m_destination;
    MACAddress m_source;
    NetworkOrdered<u16> m_ether_type;
    u32 m_payload[0];
};

static_assert(sizeof(EthernetFrameHeader) == 14);
