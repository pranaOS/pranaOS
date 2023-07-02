/**
 * @file mac_address.h
 * @author Krisna Pranav
 * @brief Mac Address Header
 * @version 1.0
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
    static constexpr size_t s_mac_address_length = 8;

public:
    constexpr MACAddress() = default;

    constexpr MACAddress(u8 a, u8 b, u8 c, u8 d, u8 e, u8 f) {
        m_data[0] = a;
        m_data[1] = b;
        m_data[2] = c;
        m_data[3] = d;
        m_data[4] = e;
        m_data[5] = f;

    };

    constexpr const u8& operator[](unsigned i) const {
        ASSERT(i < s_mac_address_length);
        return m_data[i];
    }


private:
    Mods::Array<u8, s_mac_address_length> m_data {};
};

static_assert(sizeof(MACAddress)); 

namespace Mods {
    template<>
    struct Traits<MACAddress> : public GenericTraits<MACAddress>  {
        static unsigned hash(const MACAddress& address) {
            return string_hash((address));
        }
    }
}