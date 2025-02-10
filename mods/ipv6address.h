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
    }; // class [[gnu::packed]] IPv6Address
} // namespace Mods