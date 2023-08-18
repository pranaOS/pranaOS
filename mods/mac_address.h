/**
 * @file mac_address.h
 * @author Krisna Pranav
 * @brief Mac Address Header
 * @version 6.0
 * @date 2023-07-02
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "all.h"
#include "array.h"
#include "assertions.h"
#include "string.h"
#include "types.h"


class [[gnu::packed]] MACAddress {
    static constexpr size_t s_mac_address_length = 6u;

public:
    constexpr MACAddress() = default;

    /**
     * @brief macaddress data.
     * 
     */
    constexpr MACAddress(u8 a, u8 b, u8 c, u8 d, u8 e, u8 f) {
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
     * @return constexpr const u8& 
     */
    constexpr const u8& operator[](unsigned i) const {
        ASSERT(i < s_mac_address_length);
        return m_data[i];
    }

    constexpr u8& operator[](unsigned i) {
        ASSERT(i < s_mac_address_length);
        return m_data[i];
    }

    constexpr bool operator==(const MACAddress& other) const {
        for (auto i = 0u; i < m_data.size(); ++i) {
            if (m_data[i] != other.m_data[i]) {
                return false;
            }
        }
        return true;
    }
    
    // TODO: need to implement strings
    // Mods::String to_string() const {
    //     return String::formatted("{:02x}:{:02x}:{:02x}:{:02x}:{:02x}:{:02x}", m_data[0], m_data[1], m_data[2], m_data[3], m_data[4], m_data[5]);
    // }

    constexpr bool is_zero() const {
        return Mods::allof(m_data.begin(), m_data.end(), [](const auto octet) { return octet == 0; });
    }

private:
    Mods::Array<u8, s_mac_address_length> m_data {};
};

static_assert(sizeof(MACAddress) == 6u);

namespace Mods {

    template<>
    struct Traits<MACAddress> : public GenericTraits<MACAddress> {
        static unsigned hash(const MACAddress& address) { return string_hash((const char*)&address, sizeof(address)); }
    };

}
