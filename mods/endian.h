/**
 * @file endian.h
 * @author Krisna Pranav
 * @brief Endian
 * @version 6.0
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/format.h>
#include <mods/forward.h>
#include <mods/platform.h>

namespace Mods 
{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
inline constexpr static bool HostIsLittleEndian = true;
#else
inline constexpr static bool HostIsLittleEndian = false;
#endif

/** 
 * @tparam T 
 * @param value 
 * @return ALWAYS_INLINE constexpr 
 */
template<typename T>
ALWAYS_INLINE constexpr T convert_between_host_and_little_endian(T value)
{
    if constexpr (HostIsLittleEndian || sizeof(T) == 1)
        return value;
    else if constexpr (sizeof(T) == 8)
        return static_cast<T>(__builtin_bswap64(static_cast<u64>(value)));
    else if constexpr (sizeof(T) == 4)
        return static_cast<T>(__builtin_bswap32(static_cast<u32>(value)));
    else if constexpr (sizeof(T) == 2)
        return static_cast<T>(__builtin_bswap16(static_cast<u16>(value)));
    else
        static_assert(DependentFalse<T>, "Cannot byte-swap values larger than 64-bits");
}

/**
 * @tparam T 
 * @param value 
 * @return ALWAYS_INLINE constexpr 
 */
template<typename T>
ALWAYS_INLINE constexpr T convert_between_host_and_big_endian(T value)
{
    if constexpr (sizeof(T) == 1 || !HostIsLittleEndian)
        return value;
    else if constexpr (sizeof(T) == 8)
        return static_cast<T>(__builtin_bswap64(static_cast<u64>(value)));
    else if constexpr (sizeof(T) == 4)
        return static_cast<T>(__builtin_bswap32(static_cast<u32>(value)));
    else if constexpr (sizeof(T) == 2)
        return static_cast<T>(__builtin_bswap16(static_cast<u16>(value)));
    else
        static_assert(DependentFalse<T>, "Cannot byte-swap values larger than 64-bits");
}

/**
 * @tparam T 
 * @param value 
 * @return ALWAYS_INLINE 
 */
template<typename T>
ALWAYS_INLINE T convert_between_host_and_network_endian(T value)
{
    return convert_between_host_and_big_endian(value);
}

/**
 * @tparam T 
 */
template<typename T>
class [[gnu::packed]] LittleEndian 
{
public:
    constexpr LittleEndian() = default;

    /**
     * @param value
     * 
     */
    constexpr LittleEndian(T value)
        : m_value(convert_between_host_and_little_endian(value))
    {
    }

    constexpr operator T() const 
    { 
        return convert_between_host_and_little_endian(m_value); 
    }

private:
    T m_value { 0 };
}; // class [[gnu::packed]] LittleEndian

/**
 * @tparam T 
 */
template<typename T>
class [[gnu::packed]] BigEndian 
{
public:
    constexpr BigEndian() = default;

    /**
     * @param value
     * 
     */
    constexpr BigEndian(T value)
        : m_value(convert_between_host_and_big_endian(value))
    {
    }

    constexpr operator T() const 
    { 
        return convert_between_host_and_big_endian(m_value); 
    }

private:
    T m_value { 0 };
}; // class [[gnu::packed]] BigEndian

/**
 * @tparam T 
 */
template<typename T>
using NetworkOrdered = BigEndian<T>;

/**
 * @brief Construct a new requires object
 * 
 * @tparam T 
 */
template<typename T>
requires(HasFormatter<T>) struct Formatter<LittleEndian<T>> : Formatter<T> {
};

/**
 * @brief Construct a new requires object
 * 
 * @tparam T 
 */
template<typename T>
requires(HasFormatter<T>) struct Formatter<BigEndian<T>> : Formatter<T> {
};

/**
 * @tparam T 
 */
template<typename T>
struct Traits<LittleEndian<T>> : public DefaultTraits<LittleEndian<T>> {
    static constexpr bool is_trivially_serializable() { return Traits<T>::is_trivially_serializable(); }
};

/**
 * @tparam T 
 */
template<typename T>
struct Traits<BigEndian<T>> : public DefaultTraits<BigEndian<T>> {
    static constexpr bool is_trivially_serializable() { return Traits<T>::is_trivially_serializable(); }
};

/**
 * @param v 
 * @return constexpr u16 
 */
constexpr u16 bitswap(u16 v)
{
    v = ((v >> 1) & 0x5555) | ((v & 0x5555) << 1);    
    v = ((v >> 2) & 0x3333) | ((v & 0x3333) << 2);    
    v = ((v >> 4) & 0x0F0F) | ((v & 0x0F0F) << 4);    
    return ((v >> 8) & 0x00FF) | ((v & 0x00FF) << 8); 
}

}

#if USING_MODS_GLOBALLY
using Mods::BigEndian;
using Mods::bitswap;
using Mods::LittleEndian;
using Mods::NetworkOrdered;
#endif
