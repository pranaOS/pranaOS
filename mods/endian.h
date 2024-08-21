/**
 * @file endian.h
 * @author Krisna Pranav
 * @brief Endian
 * @version 6.0
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "forward.h"
#include "platform.h"

namespace Mods {
    
    /** 
     * @tparam T 
     * @param value 
     * @return ALWAYS_INLINE constexpr 
     */
    template<typename T>
    ALWAYS_INLINE constexpr T convert_between_host_and_little_endian(T value) {
    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        return value;
    #elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        if constexpr (sizeof(T) == 8)
            return __builtin_bswap64(value);
        if constexpr (sizeof(T) == 4)
            return __builtin_bswap32(value);
        if constexpr (sizeof(T) == 2)
            return __builtin_bswap16(value);
        if constexpr (sizeof(T) == 1)
            return value;
    #endif
    }
    
    /**
     * @tparam T 
     * @param value 
     * @return ALWAYS_INLINE constexpr 
     */
    template<typename T>
    ALWAYS_INLINE constexpr T convert_between_host_and_big_endian(T value) {
    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        if constexpr (sizeof(T) == 8)
            return __builtin_bswap64(value);
        if constexpr (sizeof(T) == 4)
            return __builtin_bswap32(value);
        if constexpr (sizeof(T) == 2)
            return __builtin_bswap16(value);
        if constexpr (sizeof(T) == 1)
            return value;
    #elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        return value;
    #endif
    }

    /**
     * @tparam T 
     * @param value 
     * @return ALWAYS_INLINE 
     */
    template<typename T>
    ALWAYS_INLINE T convert_between_host_and_network_endian(T value) {
        return convert_between_host_and_big_endian(value);
    }

    template<typename T>
    class LittleEndian;

    /**
     * @tparam T 
     * @return InputStream& 
     */
    template<typename T>
    InputStream& operator>>(InputStream&, LittleEndian<T>&);

    /**
     * @tparam T 
     * @return OutputStream& 
     */
    template<typename T>
    OutputStream& operator<<(OutputStream&, LittleEndian<T>);

    template<typename T>
    class [[gnu::packed]] LittleEndian {
    public:

        /**
         * @return InputStream& 
         */
        friend InputStream& operator>><T>(InputStream&, LittleEndian<T>&);

        /**
         * @return OutputStream& 
         */
        friend OutputStream& operator<<<T>(OutputStream&, LittleEndian<T>);

        constexpr LittleEndian() {}

        constexpr LittleEndian(T value) : m_value(convert_between_host_and_little_endian(value))
        {
        }

        /**
         * @return T 
         */
        constexpr operator T() const { 
            return convert_between_host_and_little_endian(m_value); 
        }

    private:
        T m_value { 0 };
    };

    template<typename T>
    class BigEndian;

    /**
     * @tparam T 
     * @return InputStream& 
     */
    template<typename T>
    InputStream& operator>>(InputStream&, BigEndian<T>&);

    /**
     * @tparam T 
     * @return OutputStream& 
     */
    template<typename T>
    OutputStream& operator<<(OutputStream&, BigEndian<T>);

    template<typename T>
    class [[gnu::packed]] BigEndian {
    public:

        /**
         * @return InputStream& 
         */
        friend InputStream& operator>><T>(InputStream&, BigEndian<T>&);

        /**
         * @return OutputStream& 
         */
        friend OutputStream& operator<<<T>(OutputStream&, BigEndian<T>);

        constexpr BigEndian() = default;

        constexpr BigEndian(T value) : m_value(convert_between_host_and_big_endian(value))
        {
        }

        /**
         * @return T 
         */
        constexpr operator T() const { 
            return convert_between_host_and_big_endian(m_value); 
        }

    private:
        T m_value { 0 };
    };

    template<typename T>
    using NetworkOrdered = BigEndian<T>;

}

using Mods::BigEndian;
using Mods::LittleEndian;
using Mods::NetworkOrdered;
