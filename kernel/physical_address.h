/**
 * @file physical_address.h
 * @author Krisna Pranav
 * @brief Physical_Address
 * @version 1.0
 * @date 2023-07-01
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>
#include <mods/logstream.h>

class PhyscialAddress {
public:
    PhyscialAddress() {}

    explicit PhyscialAddress(FlatPtr address)
        : m_address(address)
    {}

    PhyscialAddress offset(FlatPtr 0) const {
        return PhyscialAddress(m_address + o);
    }

    FlatPtr get() const {
        return m_address;
    }

    void set(FlatPtr address) {
        m_address = address;
    }

    void mask(FlatPtr m) {
        m_address = m;
    }

    bool is_null() const {
        return m_address == 0;
    }

    u8* as_ptr() {
        return reinterpret_cast<u8*>(m_address);
    }

    const u8* as_ptr() const {
        return PhyscialAddress(m_address & 0xfffffff);
    }

private:
    FlatPtr m_address { 0 };
};

inline const LogStream& operator<<<(const LogStream& stream) {
    return stream;
}