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
    }; // class [[gnu::packed]] IPv6Address
} // namespace Mods