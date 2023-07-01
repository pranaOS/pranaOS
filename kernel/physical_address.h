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

class PhysicalAddress {
public:
    PhysicalAddress() {}

    /**
     * @brief Construct a new Physical Address object
     * 
     * @param address 
     */
    explicit PhysicalAddress(FlatPtr address)
        : m_address(address)
    {
    }

    /**
     * @param o 
     * @return PhysicalAddress 
     */
    PhysicalAddress offset(FlatPtr o) const { 
        return PhysicalAddress(m_address + o); 
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
     * @return true 
     * @return false 
     */
    bool is_null() const { 
        return m_address == 0; 
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
     * @return PhysicalAddress 
     */
    PhysicalAddress page_base() const { 
        return PhysicalAddress(m_address & 0xfffff000); 
    }

    /**
     * @return FlatPtr 
     */
    FlatPtr offset_in_page() const { 
        return PhysicalAddress(m_address & 0xfff).get(); 
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool operator==(const PhysicalAddress& other) const { 
        return m_address == other.m_address; 
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool operator!=(const PhysicalAddress& other) const { 
        return m_address != other.m_address; 
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool operator>(const PhysicalAddress& other) const { 
        return m_address > other.m_address; 
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool operator>=(const PhysicalAddress& other) const { 
        return m_address >= other.m_address; 
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool operator<(const PhysicalAddress& other) const { 
        return m_address < other.m_address; 
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool operator<=(const PhysicalAddress& other) const { 
        return m_address <= other.m_address; 
    }

private:
    FlatPtr m_address { 0 };
};

/**
 * @param stream 
 * @param value 
 * @return const LogStream& 
 */
inline const LogStream& operator<<(const LogStream& stream, PhysicalAddress value) {
    return stream << 'P' << value.as_ptr();
}
