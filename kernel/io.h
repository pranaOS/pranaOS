/**
 * @file io.h
 * @author Krisna Pranav
 * @brief IO
 * @version 6.0
 * @date 2023-06-19
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/assertions.h>
#include <mods/logstream.h>
#include <mods/string.h>
#include <mods/types.h>
#include <kernel/arch/i386/cpu.h>

namespace IO {

    /**
     * @param port 
     * @return u8 
     */
    inline u8 in8(u16 port) {
        u8 value;
        asm volatile("inb %1, %0"
                    : "=a"(value)
                    : "Nd"(port));
        return value;
    }

    /**
     * @param port 
     * @return u16 
     */
    inline u16 in16(u16 port) {
        u16 value;
        asm volatile("inw %1, %0"
                    : "=a"(value)
                    : "Nd"(port));
        return value;
    }  
    
    /**
     * @param port 
     * @return u32 
     */
    inline u32 in32(u16 port) {
        u32 value;
        asm volatile("inl %1, %0"
                    : "=a"(value)
                    : "Nd"(port));
        return value;
    }

    /**
     * @param port 
     * @param buffer 
     * @param buffer_size 
     */
    inline void repeated_in16(u16 port, u16* buffer, int buffer_size) {
        asm volatile("rep insw"
                    : "+D"(buffer), "+c"(buffer_size)
                    : "d"(port)
                    : "memory");
    }
    
    /**
     * @param port 
     * @param value 
     */
    inline void out8(u16 port, u8 value) {
        asm volatile("outb %0, %1" ::"a"(value), "Nd"(port));
    }

    /**
     * @param port 
     * @param value 
     */
    inline void out16(u16 port, u16 value) {
        asm volatile("outw %0, %1" ::"a"(value), "Nd"(port));
    }

    /**
     * @param port 
     * @param value 
     */
    inline void out32(u16 port, u32 value) {
        asm volatile("outl %0, %1" ::"a"(value), "Nd"(port));
    }

    /**
     * @param port 
     * @param data 
     * @param data_size 
     */
    inline void repeated_out16(u16 port, const u16* data, int data_size) {
        asm volatile("rep outsw"
                    : "+S"(data), "+c"(data_size)
                    : "d"(port));
    }

    /**
     * @param microseconds 
     */
    inline void delay(size_t microseconds) {
        for (size_t i = 0; i < microseconds; ++i)
            IO::in8(0x80);
    }

}

class IOAddress {
public:
    IOAddress() {}

    explicit IOAddress(u16 address) : m_address(address)
    {
    }

    /**
     * @param o 
     * @return IOAddress 
     */
    IOAddress offset(u16 o) const { 
        return IOAddress(m_address + o); 
    }

    /**
     * @return u16 
     */
    u16 get() const { 
        return m_address; 
    }

    /**
     * @param address 
     */
    void set(u16 address) { 
        m_address = address; 
    }

    /**
     * @param m 
     */
    void mask(u16 m) { 
        m_address &= m; 
    }

    /**
     * @tparam T 
     * @return ALWAYS_INLINE 
     */
    template<typename T>
    ALWAYS_INLINE T in() {
        if constexpr (sizeof(T) == 4)
            return IO::in32(get());
        if constexpr (sizeof(T) == 2)
            return IO::in16(get());
        if constexpr (sizeof(T) == 1)
            return IO::in8(get());
        ASSERT_NOT_REACHED();
    }

    /**
     * @tparam T 
     * @param value 
     * @return ALWAYS_INLINE 
     */
    template<typename T>
    ALWAYS_INLINE void out(T value) {
        if constexpr (sizeof(T) == 4) {
            IO::out32(get(), value);
            return;
        }
        if constexpr (sizeof(T) == 2) {
            IO::out16(get(), value);
            return;
        }
        if constexpr (sizeof(T) == 1) {
            IO::out8(get(), value);
            return;
        }
        ASSERT_NOT_REACHED();
    }

    /**
     * @param value 
     * @param bit_width 
     */
    inline void out(u32 value, u8 bit_width) {
        if (bit_width == 32) {
            IO::out32(get(), value);
            return;
        }
        if (bit_width == 16) {
            IO::out16(get(), value);
            return;
        }
        if (bit_width == 8) {
            IO::out8(get(), value);
            return;
        }
        ASSERT_NOT_REACHED();
    }
    
    /**
     * @return true 
     * @return false 
     */
    bool is_null() const { 
        return m_address == 0;
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool operator==(const IOAddress& other) const { 
        return m_address == other.m_address; 
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool operator!=(const IOAddress& other) const { 
        return m_address != other.m_address; 
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool operator>(const IOAddress& other) const { 
        return m_address > other.m_address; 
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool operator>=(const IOAddress& other) const { 
        return m_address >= other.m_address; 
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool operator<(const IOAddress& other) const { 
        return m_address < other.m_address; 
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool operator<=(const IOAddress& other) const { 
        return m_address <= other.m_address; 
    }

private:
    u16 m_address { 0 };
};

/**
 * @param stream 
 * @param value 
 * @return const LogStream& 
 */
inline const LogStream& operator<<(const LogStream& stream, IOAddress value) {
    return stream << "IO " << String::format("%x", value.get());
}