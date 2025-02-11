/**
 * @file mac_address.h
 * @author Krisna Pranav
 * @brief Mac Address Header
 * @version 6.0
 * @date 2023-07-02
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/all.h>
#include <mods/array.h>
#include <mods/assertions.h>
#include <mods/types.h>
#include <mods/vector.h>

#ifdef KERNEL
#include <kernel/kstring.h>
#else
#include <mods/string.h>
#endif

class [[gnu::packed]] MACAddress
{
    static constexpr size_t s_mac_address_length = 6u;

  public:
    constexpr MACAddress() = default;

    /**
     * @param a
     * @param b
     * @param c
     * @param d
     * @param e
     * @param f
     */
    constexpr MACAddress(u8 a, u8 b, u8 c, u8 d, u8 e, u8 f)
    {
        m_data[0] = a;
        m_data[1] = b;
        m_data[2] = c;
        m_data[3] = d;
        m_data[4] = e;
        m_data[5] = f;
    }

    constexpr ~MACAddress() = default;

    /**
     * @param i 
     * @return constexpr u8 const& 
     */
    constexpr u8 const& operator[](unsigned i) const
    {
        VERIFY(i < s_mac_address_length);
        return m_data[i];
    }

    /**
     * @param i 
     * @return constexpr u8& 
     */
    constexpr u8& operator[](unsigned i)
    {
        VERIFY(i < s_mac_address_length);
        return m_data[i];
    }   

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    constexpr bool operator==(MACAddress const& other) const
    {
        for(auto i = 0u; i < m_data.size(); ++i)
        {
            if(m_data[i] != other.m_data[i])
            {
                return false;
            }
        }
        return true;
    }

#ifdef KERNEL
    ErrorOr<NonnullOwnPtr<Kernel::KString>> to_string() const
    {
        return Kernel::KString::formatted("{:02x}:{:02x}:{:02x}:{:02x}:{:02x}:{:02x}", m_data[0], m_data[1], m_data[2], m_data[3], m_data[4], m_data[5]);
    }
#else
    String to_string() const
    {
        return String::formatted("{:02x}:{:02x}:{:02x}:{:02x}:{:02x}:{:02x}", m_data[0], m_data[1], m_data[2], m_data[3], m_data[4], m_data[5]);
    }
#endif

    /**
     * @param string 
     * @return Optional<MACAddress> 
     */
    static Optional<MACAddress> from_string(StringView string)
    {
        if(string.is_null())
            return {};

        auto const parts = string.split_view(":");
        if(parts.size() != 6)
            return {};

        auto a = Mods::StringUtils::convert_to_uint_from_hex(parts[0]).value_or(256);
        auto b = Mods::StringUtils::convert_to_uint_from_hex(parts[1]).value_or(256);
        auto c = Mods::StringUtils::convert_to_uint_from_hex(parts[2]).value_or(256);
        auto d = Mods::StringUtils::convert_to_uint_from_hex(parts[3]).value_or(256);
        auto e = Mods::StringUtils::convert_to_uint_from_hex(parts[4]).value_or(256);
        auto f = Mods::StringUtils::convert_to_uint_from_hex(parts[5]).value_or(256);

        if(a > 255 || b > 255 || c > 255 || d > 255 || e > 255 || f > 255)
            return {};

        return MACAddress(a, b, c, d, e, f);
    }

    /**
     * @return true 
     * @return false 
     */
    constexpr bool is_zero() const
    {
        return all_of(m_data, [](auto const octet)
                      { return octet == 0; });
    }

    /**
     * @param destination 
     */
    void copy_to(Bytes destination) const
    {
        m_data.span().copy_to(destination);
    }

  private:
    Array<u8, s_mac_address_length> m_data{};
}; // class [[gnu::packed]] MACAddress

static_assert(sizeof(MACAddress) == 6u);

namespace Mods
{
    /**
     * @tparam  
     */
    template <>
    struct Traits<MACAddress> : public GenericTraits<MACAddress>
    {
        /**
         * @param address 
         * @return unsigned 
         */
        static unsigned hash(MACAddress const& address)
        {
            return string_hash((char const*)&address, sizeof(address));
        }
    }; // struct Traits<MACAddress> : public GenericTraits<MACAddress>
} // namespace Mods