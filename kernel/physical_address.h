/**
 * @file physical_address.h
 * @author Krisna Pranav
 * @brief Physical_Address
 * @version 6.0
 * @date 2023-07-01
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/check.h>
#include <mods/format.h>
#include <mods/types.h>

typedef u64 PhysicalPtr;
typedef u64 PhysicalSize;

class PhysicalAddress 
{
public:
    /**
     * @param page_address 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE static PhysicalPtr physical_page_base(PhysicalPtr page_address) 
    { 
        return page_address & ~(PhysicalPtr)0xfff; 
    }
    
    /**
     * @param page_address 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE static size_t physical_page_index(PhysicalPtr page_address)
    {
        auto page_index = page_address >> 12;
        if constexpr (sizeof(size_t) < sizeof(PhysicalPtr))
            VERIFY(!(page_index & ~(PhysicalPtr)((size_t)-1)));
        return (size_t)(page_index);
    }

    /**
     * @brief Construct a new Physical Address object
     * 
     */
    PhysicalAddress() = default;

    /**
     * @brief Construct a new Physical Address object
     * 
     * @param address 
     */
    explicit PhysicalAddress(PhysicalPtr address)
        : m_address(address)
    {
    }

    /**
     * @param o 
     * @return PhysicalAddress 
     */
    [[nodiscard]] PhysicalAddress offset(PhysicalPtr o) const 
    { 
        return PhysicalAddress(m_address + o); 
    }

    /**
     * @param o 
     * @return true 
     * @return false 
     */
    [[nodiscard]] bool offset_addition_would_overflow(PhysicalPtr o) const 
    { 
        return Checked<PhysicalPtr>::addition_would_overflow(m_address, o); 
    }

    /**
     * @return PhysicalPtr 
     */
    [[nodiscard]] PhysicalPtr get() const 
    { 
        return m_address; 
    }

    /**
     * @param address 
     */
    void set(PhysicalPtr address) 
    { 
        m_address = address; 
    }

    /**
     * @param m 
     */
    void mask(PhysicalPtr m) 
    { 
        m_address &= m; 
    }

    [[nodiscard]] bool is_null() const 
    { 
        return m_address == 0; 
    }

    [[nodiscard]] u8* as_ptr() 
    { 
        return reinterpret_cast<u8*>(m_address); 
    }

    [[nodiscard]] const u8* as_ptr() const 
    { 
        return reinterpret_cast<u8 const*>(m_address); 
    }

    [[nodiscard]] PhysicalAddress page_base() const 
    { 
        return PhysicalAddress(physical_page_base(m_address)); 
    }

    [[nodiscard]] PhysicalPtr offset_in_page() const 
    { 
        return PhysicalAddress(m_address & 0xfff).get(); 
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool operator==(const PhysicalAddress& other) const 
    { 
        return m_address == other.m_address; 
    }
    
    bool operator!=(const PhysicalAddress& other) const 
    { 
        return m_address != other.m_address; 
    }

    bool operator>(const PhysicalAddress& other) const 
    { 
        return m_address > other.m_address; 
    }

    bool operator>=(const PhysicalAddress& other) const 
    { 
        return m_address >= other.m_address; 
    }

    bool operator<(const PhysicalAddress& other) const 
    { 
        return m_address < other.m_address; 
    }

    bool operator<=(const PhysicalAddress& other) const 
    { 
        return m_address <= other.m_address; 
    }

private:
    PhysicalPtr m_address { 0 };
}; // class PhysicalAddress 

template<>
struct Mods::Formatter<PhysicalAddress> : Mods::Formatter<FormatString> 
{
    /**
     * @param builder 
     * @param value 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> format(FormatBuilder& builder, PhysicalAddress value)
    {
        if constexpr (sizeof(PhysicalPtr) == sizeof(u64))
            return Mods::Formatter<FormatString>::format(builder, "P{:016x}", value.get());
        else
            return Mods::Formatter<FormatString>::format(builder, "P{}", value.as_ptr());
    }
};
