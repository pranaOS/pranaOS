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

#if defined(MODS_OS_MACOS)
#    include <libkern/OSByteOrder.h>
#    include <machine/endian.h>

#    define htobe16(x) OSSwapHostToBigInt16(x)
#    define htole16(x) OSSwapHostToLittleInt16(x)
#    define be16toh(x) OSSwapBigToHostInt16(x)
#    define le16toh(x) OSSwapLittleToHostInt16(x)

#    define htobe32(x) OSSwapHostToBigInt32(x)
#    define htole32(x) OSSwapHostToLittleInt32(x)
#    define be32toh(x) OSSwapBigToHostInt32(x)
#    define le32toh(x) OSSwapLittleToHostInt32(x)

#    define htobe64(x) OSSwapHostToBigInt64(x)
#    define htole64(x) OSSwapHostToLittleInt64(x)
#    define be64toh(x) OSSwapBigToHostInt64(x)
#    define le64toh(x) OSSwapLittleToHostInt64(x)

#    define __BIG_ENDIAN BIG_ENDIAN
#    define __LITTLE_ENDIAN LITTLE_ENDIAN
#    define __BYTE_ORDER BYTE_ORDER
#endif

namespace Mods 
{
    /**
     * @tparam T 
     * @param value 
     * @return ALWAYS_INLINE constexpr 
     */
    template<typename T>
    ALWAYS_INLINE constexpr T convert_between_host_and_little_endian(T value)
    {
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
    ALWAYS_INLINE constexpr T convert_between_host_and_big_endian(T value)
    {
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
    ALWAYS_INLINE T convert_between_host_and_network_endian(T value)
    {
        return convert_between_host_and_big_endian(value);
    }

    /**
     * @tparam T 
     */
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

    /**
     * @tparam T 
     */
    template<typename T>
    class [[gnu::packed]] LittleEndian 
    {
    public:
        /**
         * @return InputStream& 
         */
        friend InputStream& operator>><T>(InputStream&, LittleEndian<T>&);

        /**
         * @return OutputStream& 
         */
        friend OutputStream& operator<< <T>(OutputStream&, LittleEndian<T>);

        constexpr LittleEndian() = default;

        constexpr LittleEndian(T value)
            : m_value(convert_between_host_and_little_endian(value))
        {
        }

        /**
         * @return T 
         */
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
    
    /**
     * @tparam T 
     */
    template<typename T>
    class [[gnu::packed]] BigEndian 
    {
    public:

        /**
         * @return InputStream& 
         */
        friend InputStream& operator>><T>(InputStream&, BigEndian<T>&);

        /**
         * @return OutputStream& 
         */
        friend OutputStream& operator<< <T>(OutputStream&, BigEndian<T>);

        constexpr BigEndian() = default;

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
} // namespace Mods

using Mods::BigEndian;
using Mods::LittleEndian;
using Mods::NetworkOrdered;
