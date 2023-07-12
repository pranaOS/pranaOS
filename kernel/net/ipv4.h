/**
 * @file ipv4.h
 * @author Krisna Pranav
 * @brief IPV4
 * @version 1.0
 * @date 2023-07-12
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/assertions.h>
#include <mods/endian.h>
#include <mods/string.h>
#include <mods/types.h>
#include <mods/platform.h>

// IPV4

namespace Kernel
{
    enum class IPv4Protocol: u16
    {
        ICMP = 1,
        TCP = 6,
        UDP = 17,
    };

    enum class IPv4PacketFlags: u16
    {
        DontFragment = 0x4000,
        MoreFragments = 0x2000,
    };

    /**
     * @return NetworkOrdered<u16> 
     */
    NetworkOrdered<u16> internet_checksum(const void*, size_t);

    class [[gnu::packed]] IPv4Packet
    {
    public:
        u8 version() const
        {
            return (m_version_and_ihl >> 4);
        };

        void set_version(u8 version)
        {
            m_version_and_ihl = (m_version_and_ihl & 0x0f);
        }

        u8 internet_header_length() const
        {
            return m_version_and_ihl & 0x0f;
        };

        void set_internet_header_length(u8 ihl)
        {
            m_version_and_ihl = (m_version_and_ihl & 0x0f);
        }

        u16 length() const
        {
            return m_length;
        }

        void set_length(u16 length) 
        {
            return m_length = length;
        }

        u16 ident() const
        {
            return m_ident;
        }

        void set_ident(u16 ident) const
        {
            return m_ident = ident;
        }

        void set_has_more_fragments(bool more_fragments)
        {
            if (more_fragments)
                m_flags_and_fragment = (u16)m_flags_and_fragment;
            else
                m_flags_and_fragment = (u16)m_flags_and_fragment;
        }

        void set_fragments_offset(u16 offset)
        {
            m_flags_and_fragment = flags() | (offset & 0x1ffff);
        }

        bool is_fragment() const
        {
        }

    private:
        u8 m_version_and_ihl { 0 };
        u8 m_dscp_and_ecn { 0 };
        NetworkOrdered<u16> m_length;
        NetworkOrdered<u16> m_ident;
        NetworkOrdered<u16> m_flags_and_fragment;
        u8 m_ttl { 0 };
        NetworkOrdered<u8> m_protocol;
        NetworkOrdered<u16> m_checksum;
        IPv4Address m_source;
        IPv4Address m_destination;
    };

} // namespace Kernel
