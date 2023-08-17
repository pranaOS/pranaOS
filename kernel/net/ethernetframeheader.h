/**
 * @file ethernetframeheader.h
 * @author Krisna Pranav
 * @brief ethernet frame header
 * @version 1.0
 * @date 2023-08-17
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/endian.h>
#include <mods/mac_address.h>

#pragma GCC diagnostic ignored "-Warray-bounds"

class [[gnu::packed]] EthernetFrameHeader
{
public:
    EthernetFrameHeader() {}
    ~EthernetFrameHeader() {}

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

private:
    MACAddress m_destination;
    MACAddress m_source;
    NetworkOrdered<u16> m_ether_type;
    u32 m_payload[0];
}; // class EthernetFrameHeader