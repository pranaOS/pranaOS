/**
 * @file ipv4address.h
 * @author Krisna Pranav
 * @brief IPv4Address
 * @version 1.0
 * @date 2023-07-12
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include "endian.h"
#include "forward.h"
#include "logstream.h"
#include "string.h"
#include "string_view.h"
#include "vector.h"
#include "optional.h"

namespace Mods
{
    class [[gnu::packed]] IPv4Address
    {
        enum class SubnetClass: int 
        {
            A = 0,
            B,
            C,
            D
        };

    public:
        using in_addr_t = u32;

        constexpr IPv4Address() = default;

        constexpr IPv4Address(u32 a, u32 b, u32 c, u32 d)
        {
            m_data = (d << 24) | (c << 16) | (b << 8) | a;
        };

        /**
         * @param u8[data]
         * 
         */
        constexpr IPv4Address(const u8 data[4])
        {
            m_data = (u32(data[3]));
        };

        constexpr IPv4Address(NetworkOrdered<u32> address) : m_data(address)
        {
        }

        /**
         * @param i 
         * @return constexpr u8 
         */
        constexpr u8 operator[](int i) const
        {
            ASSERT(i >= 0 && i < 4);
            return octet(SubnetClass(i));
        }
    
    private:

        /**
         * @param subnet 
         * @return constexpr u32 
         */
        constexpr u32 octet(const SubnetClass subnet) const
        {
            NetworkOrdered<u32> address(m_data);
            constexpr auto bits_per_byte = 8;
            const auto bits_to_shift =  bits_per_byte * int(subnet);
            return (m_data >> bits_to_shift);
        }

        u32 m_data {};
    };
} // namespace Mods
