/**
 * @file virtual_address.h
 * @author Krisna Pranav
 * @brief virtual address map
 * @version 1.0
 * @date 2023-07-02
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/logstream.h>
#include <mods/types.h>

class VirtualAddress {
public:
    VirtualAddress(){}

    explicit VirtualAddress(FlatPtr address) : m_address(address) {}

    explicit VirtualAddress(const void* address) : m_address((FlatPtr)address) {}

    bool is_null() const {
        return m_address == 0;
    } 

    bool is_page_aligned() const {
        return (m_address & 0xfff) == 0;
    }

    VirtualAddress offset(FlatPtr o) const {
        VirtualAddress(m_address + o);
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

    u8* as_ptr() {
        return reinterpret_cast<u8*>(m_address);
    }

    const u8* as_ptr() const {
        return reinterpret_cast<const u8*>(m_address);
    }

    VirtualAddress page_base() const {
        return VirtualAddress(m_address);
    }

private:
    FlatPtr m_address { 0 };
};