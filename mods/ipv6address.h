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

#ifdef KERNEL
#include <mods/error.h>
#include <kernel/kstring.h>
#else
#include <mods/string.h>
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

        /**
         * @return data
         * 
         */
        constexpr IPv6Address(in6_addr_t const& data)
        {
            for(size_t i = 0; i < 16; i++)
                m_data[i] = data[i];
        }

        /**
         * @param ipv4_address
         * 
         */
        constexpr IPv6Address(IPv4Address const& ipv4_address)
        {
            m_data[10] = 0xff;
            m_data[11] = 0xff;
            m_data[12] = ipv4_address[0];
            m_data[13] = ipv4_address[1];
            m_data[14] = ipv4_address[2];
            m_data[15] = ipv4_address[3];
        }

        /**
         * @param i 
         * @return constexpr u16 
         */
        constexpr u16 operator[](int i) const
        {
            return group(i);
        }

    #ifdef KERNEL
        ErrorOr<NonnullOwnPtr<Kernel::KString>> to_string() const
    #else
        String to_string() const
    #endif
        {
            if(is_zero())
            {
    #ifdef KERNEL
                return Kernel::KString::try_create("::"sv);
    #else
                return "::"sv;
    #endif
            }

            StringBuilder builder;

            if(is_ipv4_mapped())
            {
    #ifdef KERNEL
                return Kernel::KString::formatted("::ffff:{}.{}.{}.{}", m_data[12], m_data[13], m_data[14], m_data[15]);
    #else
                return String::formatted("::ffff:{}.{}.{}.{}", m_data[12], m_data[13], m_data[14], m_data[15]);
    #endif
            }

            Optional<int> longest_zero_span_start;
            int zero_span_length = 0;

            for(int i = 0; i < 8;)
            {
                if(group(i) != 0)
                {
                    i++;
                    continue;
                }
                
                int contiguous_zeros = 1;

                for(int j = i + 1; j < 8; j++)
                {
                    if(group(j) != 0)
                        break;
                    contiguous_zeros++;
                }

                if(!longest_zero_span_start.has_value() || longest_zero_span_start.value() < contiguous_zeros)
                {
                    longest_zero_span_start = i;
                    zero_span_length = contiguous_zeros;
                }

                i += contiguous_zeros;
            }

            for(int i = 0; i < 8;)
            {
                if(longest_zero_span_start.has_value() && longest_zero_span_start.value() == i)
                {
                    if(longest_zero_span_start.value() + zero_span_length >= 8)
                        builder.append("::"sv);
                    else
                        builder.append(':');
                    i += zero_span_length;
                    continue;
                }

                if(i == 0)
                    builder.appendff("{:x}", group(i));
                else
                    builder.appendff(":{:x}", group(i));

                i++;
            }
    #ifdef KERNEL
            return Kernel::KString::try_create(builder.string_view());
    #else
            return builder.string_view();
    #endif
        }

        /**
         * @param string 
         * @return Optional<IPv6Address> 
         */
        static Optional<IPv6Address> from_string(StringView string)
        {
            if(string.is_null())
                return {};

            auto const parts = string.split_view(':', true);

            if(parts.is_empty())
                return {};

            if(parts.size() > 9)
            {
                return {};
            }

            if(parts.size() >= 4 && parts[parts.size() - 1].contains('.'))
            {
                auto is_ipv4_prefix = [&]()
                {
                    auto separator_part = parts[parts.size() - 2].trim_whitespace();
                    if(separator_part.is_empty())
                        return false;
                    auto separator_value = StringUtils::convert_to_uint_from_hex(separator_part);

                    if(!separator_value.has_value() || separator_value.value() != 0xffff)
                        return false;

                    for(size_t i = 0; i < parts.size() - 2; i++)
                    {
                        auto part = parts[i].trim_whitespace();
                        if(part.is_empty())
                            continue;
                        auto value = StringUtils::convert_to_uint_from_hex(part);
                        if(!value.has_value() || value.value() != 0)
                            return false;
                    }
                    return true;
                };

                if(is_ipv4_prefix())
                {
                    auto ipv4_address = IPv4Address::from_string(parts[parts.size() - 1]);
                    if(ipv4_address.has_value())
                        return IPv6Address(ipv4_address.value());
                    return {};
                }
            }

            in6_addr_t addr{};
            int group = 0;
            int have_groups = 0;
            bool found_compressed = false;

            for(size_t i = 0; i < parts.size();)
            {
                auto trimmed_part = parts[i].trim_whitespace();

                if(trimmed_part.is_empty())
                {
                    if(found_compressed)
                        return {};
                    int empty_parts = 1;
                    bool is_leading = (i == 0);
                    bool is_trailing = false;
                    for(size_t j = i + 1; j < parts.size(); j++)
                    {
                        if(!parts[j].trim_whitespace().is_empty())
                            break;
                        empty_parts++;
                        if(j == parts.size() - 1)
                            is_trailing = true;
                    }
                    if(is_leading && is_trailing)
                    {
                        if(empty_parts > 3)
                            return {};
                        return IPv6Address();
                    }
                    if(is_leading || is_trailing)
                    {
                        if(empty_parts > 2)
                            return {};
                    }
                    else if(empty_parts > 1)
                    {
                        return {};
                    }

                    int remaining_parts = parts.size() - empty_parts - have_groups;
                    found_compressed = true;
                    group = 8 - remaining_parts;
                    VERIFY(group >= 0);
                    i += empty_parts;
                    continue;
                }
                else
                {
                    i++;
                }
                auto part = StringUtils::convert_to_uint_from_hex(trimmed_part);
                if(!part.has_value() || part.value() > 0xffff)
                    return {};

                if(++have_groups > 8)
                    return {};

                VERIFY(group < 8);
                addr[group * sizeof(u16)] = (u8)(part.value() >> 8);
                addr[group * sizeof(u16) + 1] = (u8)part.value();
                group++;
            }

            return IPv6Address(addr);
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
            for(auto& d : m_data)
            {
                if(d != 0)
                    return false;
            }
            return true;
        }

        /**
         * @return true 
         * @return false 
         */
        constexpr bool is_ipv4_mapped() const
        {
            if(m_data[0] || m_data[1] || m_data[2] || m_data[3] || m_data[4] || m_data[5] || m_data[6] || m_data[7] || m_data[8] || m_data[9])
                return false;
            if(m_data[10] != 0xff || m_data[11] != 0xff)
                return false;
            return true;
        }

        /**
         * @return Optional<IPv4Address> 
         */
        Optional<IPv4Address> ipv4_mapped_address() const
        {
            if(is_ipv4_mapped())
                return IPv4Address(m_data[12], m_data[13], m_data[14], m_data[15]);
            return {};
        }

    private:
        /**
         * @param i 
         * @return constexpr u16 
         */
        constexpr u16 group(unsigned i) const
        {
            VERIFY(i < 8);
            return ((u16)m_data[i * sizeof(u16)] << 8) | m_data[i * sizeof(u16) + 1];
        }

        in6_addr_t m_data{};
    }; // class [[gnu::packed]] IPv6Address

    static_assert(sizeof(IPv6Address) == 16);

    template <>
    struct Traits<IPv6Address> : public GenericTraits<IPv6Address>
    {
        /**
         * @param address 
         * @return constexpr unsigned 
         */
        static constexpr unsigned hash(IPv6Address const& address)
        {
            unsigned h = 0;

            for(int group = 0; group < 8; group += 2)
            {
                u32 two_groups = ((u32)address[group] << 16) | (u32)address[group + 1];
                if(group == 0)
                    h = int_hash(two_groups);
                else
                    h = pair_int_hash(h, two_groups);
            }
            return h;
        } 
    }; // struct Traits<IPv6Address> : public GenericTraits<IPv6Address>

    #ifdef KERNEL
    template <>
    struct Formatter<IPv6Address> : Formatter<ErrorOr<NonnullOwnPtr<Kernel::KString>>>
    {
        ErrorOr<void> format(FormatBuilder& builder, IPv6Address const& value)
        {
            return Formatter<ErrorOr<NonnullOwnPtr<Kernel::KString>>>::format(builder, value.to_string());
        }
    };
    #else
    template <>
    struct Formatter<IPv6Address> : Formatter<String>
    {
        ErrorOr<void> format(FormatBuilder& builder, IPv6Address const& value)
        {
            return Formatter<String>::format(builder, value.to_string());
        }
    };
    #endif

} // namespace Mods

using Mods::IPv6Address;
