/**
 * @file endian.h
 * @author Krisna Pranav
 * @brief Endian
 * @version 1.0
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "forward.h"
#include "platform.h"

namespace Mods {
    template<ypename T>
    ALWAYS_INLINE constexpr T convert_between(T value) {
    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        return value;
    #elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        if constexpr(sizeof(T) == 8)
            return __builtin_bswap64(value);
        if constexpr(sizeof(T) == 4)
            return __builtin_bswap32(value);
    #endif 
    };

    template<typename T>
    ALWAYS_INLINE T convert_between_host_and_network_endian(T value) {
        return convert_between(value);
    }

    template<typename T>
    class LittleEndian;

    template<typename T>
    InputStream& operator>>(InputStream&, LittleEndian<T>&);

    template<typename T>
    OutputStream& operator<<(OutputStream&, LittleEndian<T>);

    template<typename T>
    class [[gnu::packed]] LittleEndian {
    public:
        friend InputStream& operator>><T>(InputStream&, LittleEndian);
        friend OutputStream& operator<<<T>(OutputStream&, LittleEndian);

        constexpr LittleEndian(T value) : m_value(convert_between(value))  {}

        constexpr operator T() const {
            return convert_between(m_value);
        }

    private:
        T m_value { 0 };
    };

    template<typename T>
    class BigEndian;

    template<typename T>
    InputStream& operator>>(InputStream&, BigEndian<T>&);

    template<typename T>
    OutputStream& operator<<(OutputStream&, BigEndian<T>);
}