/**
 * @file ipv4address.h
 * @author Krisna Pranav
 * @brief IPv4Address
 * @version 6.0
 * @date 2023-07-12
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

#ifdef KERNEL
#include <mods/error.h>
#include <kernel/kstring.h>
#else
#include <mods/string.h>
#endif

namespace Mods
{
    class [[gnu::packed]] IPv4Address
    {
        enum class SubnetClass : int
        {
            A = 0,
            B,
            C,
            D
        }; // enum class SubnetClass : int
 
    public:
        using in_addr_t = u32;

        constexpr IPv4Address() = default;

        constexpr IPv4Address(u32 a, u32 b, u32 c, u32 d)
        {
            m_data = (d << 24) | (c << 16) | (b << 8) | a;
        }

        constexpr IPv4Address(const u8 data[4])
        {
            m_data = (u32(data[3]) << 24) | (u32(data[2]) << 16) | (u32(data[1]) << 8) | u32(data[0]);
        }

        constexpr IPv4Address(NetworkOrdered<u32> address)
            : m_data(address)
        {
        }

        /**
         * @param i 
         * @return constexpr u8 
         */
        constexpr u8 operator[](int i) const
        {
            VERIFY(i >= 0 && i < 4);
            return octet(SubnetClass(i));
        }

    #ifdef KERNEL
        ErrorOr<NonnullOwnPtr<Kernel::KString>> to_string() const
        {
            return Kernel::KString::formatted("{}.{}.{}.{}",
                                            octet(SubnetClass::A),
                                            octet(SubnetClass::B),
                                            octet(SubnetClass::C),
                                            octet(SubnetClass::D));
        }
    #else
        String to_string() const
        {
            return String::formatted("{}.{}.{}.{}",
                                    octet(SubnetClass::A),
                                    octet(SubnetClass::B),
                                    octet(SubnetClass::C),
                                    octet(SubnetClass::D));
        }

        /**
         * @return String 
         */
        String to_string_reversed() const
        {
            return String::formatted("{}.{}.{}.{}",
                                    octet(SubnetClass::D),
                                    octet(SubnetClass::C),
                                    octet(SubnetClass::B),
                                    octet(SubnetClass::A));
        }
    #endif
        
        /**
         * @param string 
         * @return Optional<IPv4Address> 
         */
        static Optional<IPv4Address> from_string(StringView string)
        {
            if(string.is_null())
                return {};

            auto const parts = string.split_view('.');

            u32 a{};
            u32 b{};
            u32 c{};
            u32 d{};

            if(parts.size() == 1)
            {
                d = parts[0].to_uint().value_or(256);
            }
            else if(parts.size() == 2)
            {
                a = parts[0].to_uint().value_or(256);
                d = parts[1].to_uint().value_or(256);
            }
            else if(parts.size() == 3)
            {
                a = parts[0].to_uint().value_or(256);
                b = parts[1].to_uint().value_or(256);
                d = parts[2].to_uint().value_or(256);
            }
            else if(parts.size() == 4)
            {
                a = parts[0].to_uint().value_or(256);
                b = parts[1].to_uint().value_or(256);
                c = parts[2].to_uint().value_or(256);
                d = parts[3].to_uint().value_or(256);
            }
            else
            {
                return {};
            }

            if(a > 255 || b > 255 || c > 255 || d > 255)
                return {};
            return IPv4Address(a, b, c, d);
        }

        constexpr in_addr_t to_in_addr_t() const
        {
            return m_data;
        }

        constexpr u32 to_u32() const
        {
            return m_data;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator==(IPv4Address const& other) const = default;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator!=(IPv4Address const& other) const = default;

        constexpr bool is_zero() const
        {
            return m_data == 0u;
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
            auto const bits_to_shift = bits_per_byte * int(subnet);
            return (m_data >> bits_to_shift) & 0x0000'00FF;
        }

        u32 m_data{};
    }; // class [[gnu::packed]] IPv4Address

    static_assert(sizeof(IPv4Address) == 4);

    /**
     * @tparam  
     */
    template <>
    struct Traits<IPv4Address> : public GenericTraits<IPv4Address>
    {
        /**
         * @param address 
         * @return constexpr unsigned 
         */
        static constexpr unsigned hash(IPv4Address const& address)
        {
            return int_hash(address.to_u32());
        }
    }; // struct Traits<IPv4Address> : public GenericTraits<IPv4Address>

    #ifdef KERNEL
    template <>
    struct Formatter<IPv4Address> : Formatter<ErrorOr<NonnullOwnPtr<Kernel::KString>>>
    {
        ErrorOr<void> format(FormatBuilder& builder, IPv4Address value)
        {
            return Formatter<ErrorOr<NonnullOwnPtr<Kernel::KString>>>::format(builder, value.to_string());
        }
    };
    #else
    template <>
    struct Formatter<IPv4Address> : Formatter<String>
    {
        ErrorOr<void> format(FormatBuilder& builder, IPv4Address value)
        {
            return Formatter<String>::format(builder, value.to_string());
        }
    };
    #endif
} // namespace Mods

using Mods::IPv4Address;
