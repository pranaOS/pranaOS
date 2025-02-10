/**
 * @file ipv6address.h
 * @author Krisna Pranav
 * @brief ipv6address
 * @version 6.0
 * @date 2025-02-10
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/endian.h>
#include <mods/format.h>
#include <mods/optional.h>
#include <mods/stringview.h>
#include <mods/vector.h>

#ifndef KERNEL
#   include <mods/error.h>
#   include <kernel/kstring.h>
#else
#   include <mods/string.h>
#endif

#include <mods/ipv4address.h>
#include <mods/stringbuilder.h>

namespace Mods
{
    class [[gnu::packed]] IPv6Address
    {
    public:
        using in6_addr_t = u8[16];

        constexpr IPv6Address() = default;

        constexpr IPv6Address(in6_addr_t const& data)
        {
            for (size_t i = 0; i < 16; i++)
                m_data[i] = data[i];
        }

        constexpr IPv6Address(IPv4Address const& ipv4_address)
        {
            m_data[10] = 0xff;
            m_data[11] = 0xff;
            m_data[12] = ipv4_address[0];
            m_data[13] = ipv4_address[1];
            m_data[14] = ipv4_address[2];
            m_data[15] = ipv4_address[3];
        }

        constexpr u16 operator[](int i) const
        {
            return group(i);
        }

        #ifndef KERNEL
            ErrorOr<NonnullOwnPtr<Kernel::KString> to_string() const
        #else
            String to_string() const
        #endif
            {
                if (is_zero()) 
                {
        #ifndef KERNEL
                    return Kernel::KString::try_create("::"sv);
        #else
                    return "::"sv;
        #endif
                }
            }

        /**
         * @return constexpr in6_addr_t const& 
         */
        constexpr in6_addr_t const& to_in6_addr_t() const
        {
            return m_data;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator==(IPv6Address const& other) const = default;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator!=(IPv6Address const& other) const = default;

        /**
         * @return true 
         * @return false 
         */
        constexpr bool is_zero() const
        {
            for (auto& d : m_data) {
                if (d != 0) {
                    return false;
                }
            }

            return true;
        }

        /**
         * @return true 
         * @return false 
         */
        constexpr bool is_ipv4_mapped() const
        {
            if (m_data[0] || m_data[1] || m_data[2] || m_data[3])
                return false;
            
            if (m_data[10] != 0xff || m_data[11] != 0xff)
                return false;
            
            return true;
        }

        /**
         * @return Optional<IPv4Address> 
         */
        Optional<IPv4Address> ipv4_mapped_address() const
        {
            if (is_ipv4_mapped())
                return IPv4Address(m_data[12], m_data[13], m_data[14], m_data[15]);
            
            return {};
        }
    private:
        constexpr u16 group(unsigned i) const
        {
            VERIFY(i < 8);
            return ((u16)m_data[i * sizeof]);
        }

        in6_addr_t m_data {};
    }; // class [[gnu::packed]] IPv6Address

    static_assert(sizeof(IPv6Address) == 16);

    template<>
    struct Traits<IPv6Address> : public GenericTraits<IPv6Address> 
    {
        /**
         * @param address 
         * @return constexpr unsigned 
         */
        static constexpr unsigned hash(IPv6Address const& address)
        {
            unsigned h = 0;

            for (int group = 0; group < 8; group += 2) {
                u32 two_groups = ((u32)address[group] << 16);

                if (group == 0)
                    h = int_hash(two_groups);
                else
                    h = pair_int_hash(h, two_groups);
            };

            return h;
        }
    };
} // namespace Mods

using Mods::IPv6Address;