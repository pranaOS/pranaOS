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

#include <mods/format.h>
#include <mods/types.h>

class VirtualAddress 
{
public:
    /**
     * @brief Construct a new Virtual Address object
     * 
     */
    VirtualAddress() = default;

    constexpr explicit VirtualAddress(FlatPtr address)
        : m_address(address)
    { }

    /**
     * @param address 
     */
    explicit VirtualAddress(const void* address)
        : m_address((FlatPtr)address)
    { }

    /**
     * @return true 
     * @return false 
     */
    [[nodiscard]] constexpr bool is_null() const 
    { 
        return m_address == 0; 
    }

    [[nodiscard]] constexpr bool is_page_aligned() const 
    { 
        return (m_address & 0xfff) == 0; 
    }

    /**
     * @param o 
     * @return constexpr VirtualAddress 
     */
    [[nodiscard]] constexpr VirtualAddress offset(FlatPtr o) const 
    { 
        return VirtualAddress(m_address + o); 
    }

    /**
     * @return constexpr FlatPtr 
     */
    [[nodiscard]] constexpr FlatPtr get() const 
    { 
        return m_address; 
    }

    /**
     * @param address 
     */
    void set(FlatPtr address) 
    { 
        m_address = address; 
    }

    /**
     * @param m 
     */
    void mask(FlatPtr m) 
    { 
        m_address &= m; 
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool operator<=(const VirtualAddress& other) const 
    { 
        return m_address <= other.m_address; 
    }
    
    bool operator>=(const VirtualAddress& other) const 
    { 
        return m_address >= other.m_address; 
    }

    bool operator>(const VirtualAddress& other) const 
    { 
        return m_address > other.m_address; 
    }

    bool operator<(const VirtualAddress& other) const 
    { 
        return m_address < other.m_address; 
    }

    bool operator==(const VirtualAddress& other) const 
    { 
        return m_address == other.m_address; 
    }

    bool operator!=(const VirtualAddress& other) const 
    { 
        return m_address != other.m_address; 
    }

    /**
     * @return u8* 
     */
    [[nodiscard]] u8* as_ptr() 
    { 
        return reinterpret_cast<u8*>(m_address); 
    }

    /**
     * @return const u8* 
     */
    [[nodiscard]] const u8* as_ptr() const 
    { 
        return reinterpret_cast<const u8*>(m_address); 
    }

    [[nodiscard]] VirtualAddress page_base() const 
    { 
        return VirtualAddress(m_address & ~(FlatPtr)0xfffu); 
    }

private:
    FlatPtr m_address { 0 };
};

/**
 * @param a 
 * @param b 
 * @return VirtualAddress 
 */
inline VirtualAddress operator-(const VirtualAddress& a, const VirtualAddress& b)
{
    return VirtualAddress(a.get() - b.get());
}

template<>
struct Mods::Formatter<VirtualAddress> : Mods::Formatter<FormatString> 
{
    ErrorOr<void> format(FormatBuilder& builder, const VirtualAddress& value)
    {
        return Mods::Formatter<FormatString>::format(builder, "V{}", value.as_ptr());
    }
};
