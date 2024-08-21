/**
 * @file ipv4.h
 * @author Krisna Pranav
 * @brief IPV4
 * @version 6.0
 * @date 2023-07-12
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/assertions.h>
#include <mods/endian.h>
#include <mods/ipv4address.h>
#include <mods/string.h>
#include <mods/types.h>
#include <mods/platform.h>
#include <mods/string_view.h>

namespace Kernel {
    /// @brief IPv4Protocol
    enum class IPv4Protocol : u16 
    {
        ICMP = 1,
        TCP = 6,
        UDP = 17,
    };

    /// @brief IPv4PacketFlags
    enum class IPv4PacketFlags : u16 
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

        /**
         * @return u8 
         */
        u8 version() const 
        { 
            return (m_version_and_ihl >> 4) & 0xf; 
        }

        /**
         * @param version 
         */
        void set_version(u8 version) 
        { 
            m_version_and_ihl = (m_version_and_ihl & 0x0f) | (version << 4); 
        }

        /**
         * @return u8 
         */
        u8 internet_header_length() const 
        { 
            return m_version_and_ihl & 0xf; 
        }

        /**
         * @param ihl 
         */
        void set_internet_header_length(u8 ihl) 
        { 
            m_version_and_ihl = (m_version_and_ihl & 0xf0) | (ihl & 0x0f); 
        }

        /**
         * @return u16 
         */
        u16 length() const 
        { 
            return m_length; 
        }

        /**
         * @param length 
         */
        void set_length(u16 length) 
        { 
            m_length = length; 
        }

        /**
         * @return u16 
         */
        u16 ident() const 
        { 
            return m_ident; 
        }

        /**
         * @param ident 
         */
        void set_ident(u16 ident) 
        { 
            m_ident = ident; 
        }

        /**
         * @return u8 
         */
        u8 ttl() const 
        { 
            return m_ttl; 
        }

        /**
         * @param ttl 
         */
        void set_ttl(u8 ttl) 
        { 
            m_ttl = ttl; 
        }

        /**
         * @return u8 
         */
        u8 protocol() const 
        { 
            return m_protocol; 
        }

        /**
         * @param protocol 
         */
        void set_protocol(u8 protocol) 
        { 
            m_protocol = protocol; 
        }

        /**
         * @return u16 
         */
        u16 checksum() const 
        { 
            return m_checksum; 
        }

        /**
         * @param checksum 
         */
        void set_checksum(u16 checksum) 
        { 
            m_checksum = checksum; 
        }

        /**
         * @return const IPv4Address& 
         */
        const IPv4Address& source() const 
        { 
            return m_source; 
        }

        /**
         * @param address 
         */
        void set_source(const IPv4Address& address) 
        { 
            m_source = address; 
        }

        /**
         * @return const IPv4Address& 
         */
        const IPv4Address& destination() const 
        { 
            return m_destination; 
        }

        /**
         * @param address 
         */
        void set_destination(const IPv4Address& address) 
        { 
            m_destination = address; 
        }

        /**
         * @return void* 
         */
        void* payload() 
        { 
            return this + 1; 
        }

        /**
         * @return const void* 
         */
        const void* payload() const 
        { 
            return this + 1; 
        }

        /**
         * @return u16 
         */
        u16 flags_and_fragment() const 
        { 
            return m_flags_and_fragment; 
        }

        /**
         * @return u16 
         */
        u16 fragment_offset() const { 
            return ((u16)m_flags_and_fragment & 0x1fff); 
        }

        /**
         * @return u16 
         */
        u16 flags() const { 
            return (((u16)m_flags_and_fragment) & (((u16)IPv4PacketFlags::MoreFragments) | ((u16)IPv4PacketFlags::DontFragment))); 
        }

        /**
         * @param more_fragments 
         */
        void set_has_more_fragments(bool more_fragments)
        {
            if (more_fragments)
                m_flags_and_fragment = (u16)m_flags_and_fragment | ((u16)IPv4PacketFlags::MoreFragments);
            else
                m_flags_and_fragment = (u16)m_flags_and_fragment & ((u16)IPv4PacketFlags::MoreFragments);
        }

        /**
         * @param offset 
         */
        void set_fragment_offset(u16 offset)
        {
            m_flags_and_fragment = flags() | (offset & 0x1fff);
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_a_fragment() const
        {
            return (((u16)m_flags_and_fragment) & ((u16)IPv4PacketFlags::MoreFragments)) || ((u16)m_flags_and_fragment & 0x1fff);
        }

        /**
         * @return u16 
         */
        u16 payload_size() const 
        { 
            return m_length - sizeof(IPv4Packet); 
        }

        /**
         * @return NetworkOrdered<u16> 
         */
        NetworkOrdered<u16> compute_checksum() const
        {
            ASSERT(!m_checksum);
            return internet_checksum(this, sizeof(IPv4Packet));
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

    /**
     * @brief Construct a new static assert object
     * 
     */
    static_assert(sizeof(IPv4Packet) == 20);

    /**
     * @param stream 
     * @param packet 
     * @return const LogStream& 
     */
    const LogStream& operator<<(const LogStream& stream, const IPv4Packet& packet);

    /**
     * @param ptr 
     * @param count 
     * @return NetworkOrdered<u16> 
     */
    inline NetworkOrdered<u16> internet_checksum(const void* ptr, size_t count)
    {
        u32 checksum = 0;
        auto* w = (const u16*)ptr;
        while (count > 1) {
            checksum += Mods::convert_between_host_and_network_endian(*w++);
            if (checksum & 0x80000000)
                checksum = (checksum & 0xffff) | (checksum >> 16);
            count -= 2;
        }
        while (checksum >> 16)
            checksum = (checksum & 0xffff) + (checksum >> 16);
        return ~checksum & 0xffff;
    }

}
