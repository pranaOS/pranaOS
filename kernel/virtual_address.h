/**
 * @file virtual_address.h
 * @author Krisna Pranav
 * @brief virtual address map
 * @version 6.0
 * @date 2023-07-02
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/logstream.h>
#include <mods/types.h>


class VirtualAddress {
public:

    /**
     * @brief Construct a new Virtual Address object
     * 
     */
    VirtualAddress() {}

    /**
     * @brief Construct a new Virtual Address object
     * 
     * @param address 
     */
    explicit VirtualAddress(FlatPtr address) : m_address(address) {}

    /**
     * @brief Construct a new Virtual Address object
     * 
     * @param address 
     */
    explicit VirtualAddress(const void* address) : m_address((FlatPtr)address) {}

    /**
     * @return true 
     * @return false 
     */
    bool is_null() const { 
        return m_address == 0; 
    }

    /**
     * @return true 
     * @return false 
     */
    bool is_page_aligned() const { 
        return (m_address & 0xfff) == 0; 
    }

    /**
     * @param o 
     * @return VirtualAddress 
     */
    VirtualAddress offset(FlatPtr o) const { 
        return VirtualAddress(m_address + o); 
    }

    /**
     * @return FlatPtr 
     */
    FlatPtr get() const { 
        return m_address; 
    }

    /**
     * @param address 
     */
    void set(FlatPtr address) { 
        m_address = address; 
    }

    /**
     * @param m 
     */
    void mask(FlatPtr m) { 
        m_address &= m; 
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool operator<=(const VirtualAddress& other) const { 
        return m_address <= other.m_address; 
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool operator>=(const VirtualAddress& other) const { 
        return m_address >= other.m_address; 
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool operator>(const VirtualAddress& other) const { 
        return m_address > other.m_address; 
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool operator<(const VirtualAddress& other) const { 
        return m_address < other.m_address; 
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool operator==(const VirtualAddress& other) const { 
        return m_address == other.m_address; 
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool operator!=(const VirtualAddress& other) const { 
        return m_address != other.m_address; 
    }

    /**
     * @return u8* 
     */
    u8* as_ptr() { 
        return reinterpret_cast<u8*>(m_address); 
    }

    /**
     * @return const u8* 
     */
    const u8* as_ptr() const { 
        return reinterpret_cast<const u8*>(m_address); 
    }

    /**
     * @return VirtualAddress 
     */
    VirtualAddress page_base() const { 
        return VirtualAddress(m_address & 0xfffff000); 
    }

private:
    FlatPtr m_address { 0 };
};

/**
 * @param a 
 * @param b 
 * @return VirtualAddress 
 */
inline VirtualAddress operator-(const VirtualAddress& a, const VirtualAddress& b) {
    return VirtualAddress(a.get() - b.get());
}

/**
 * @param stream 
 * @param value 
 * @return const LogStream& 
 */
inline const LogStream& operator<<(const LogStream& stream, VirtualAddress value) {
    return stream << 'V' << value.as_ptr();
}
