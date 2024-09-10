/**
 * @file builtinwrappers.h
 * @author Krisna Pranav
 * @brief built in wrappers
 * @version 6.0p
 * @date 2024-09-10
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "concept.h"

/**
 * @tparam IntType 
 * @param value 
 * @return constexpr int 
 */
template<Unsigned IntType>
inline constexpr int popcount(IntType value)
{
#if defined(__GNUC__) || defined(__clang__)
    static_assert(sizeof(IntType) <= sizeof(unsigned long long));
    if constexpr (sizeof(IntType) <= sizeof(unsigned int))
        return __builtin_popcount(value);
    if constexpr (sizeof(IntType) == sizeof(unsigned long))
        return __builtin_popcountl(value);
    if constexpr (sizeof(IntType) == sizeof(unsigned long long))
        return __builtin_popcountll(value);
    VERIFY_NOT_REACHED();
#else
    int ones = 0;
    for (size_t i = 0; i < 8 * sizeof(IntType); ++i) {
        if ((val >> i) & 1) {
            ++ones;
        }
    }
    return ones;
#endif
}

/**
 * @tparam IntType 
 * @param value 
 * @return constexpr int 
 */
template<Unsigned IntType>
inline constexpr int count_trailing_zeroes(IntType value)
{
#if defined(__GNUC__) || defined(__clang__)
    static_assert(sizeof(IntType) <= sizeof(unsigned long long));
    if constexpr (sizeof(IntType) <= sizeof(unsigned int))
        return __builtin_ctz(value);
    if constexpr (sizeof(IntType) == sizeof(unsigned long))
        return __builtin_ctzl(value);
    if constexpr (sizeof(IntType) == sizeof(unsigned long long))
        return __builtin_ctzll(value);
    VERIFY_NOT_REACHED();
#else
    for (size_t i = 0; i < 8 * sizeof(IntType); ++i) {
        if ((val >> i) & 1) {
            return i;
        }
    }
    return 8 * sizeof(IntType);
#endif
}

/**
 * @tparam IntType 
 * @param value 
 * @return constexpr int 
 */
template<Unsigned IntType>
inline constexpr int count_trailing_zeroes_safe(IntType value)
{
    if (value == 0)
        return 8 * sizeof(IntType);
    return count_trailing_zeroes(value);
}

/**
 * @tparam IntType 
 * @param value 
 * @return constexpr int 
 */
template<Unsigned IntType>
inline constexpr int count_leading_zeroes(IntType value)
{
#if defined(__GNUC__) || defined(__clang__)
    static_assert(sizeof(IntType) <= sizeof(unsigned long long));
    if constexpr (sizeof(IntType) <= sizeof(unsigned int))
        return __builtin_clz(value) - (32 - (8 * sizeof(IntType)));
    if constexpr (sizeof(IntType) == sizeof(unsigned long))
        return __builtin_clzl(value);
    if constexpr (sizeof(IntType) == sizeof(unsigned long long))
        return __builtin_clzll(value);
    VERIFY_NOT_REACHED();
#else
    for (size_t i = (8 * sizeof(IntType)) - 1; i < 8 * sizeof(IntType); --i) {
        if ((val >> i) & 1) {
            return i;
        }
    }
    return 8 * sizeof(IntType);
#endif
}

/**
 * @tparam IntType 
 * @param value 
 * @return constexpr int 
 */
template<Unsigned IntType>
inline constexpr int count_leading_zeroes_safe(IntType value)
{
    if (value == 0)
        return 8 * sizeof(IntType);
    return count_leading_zeroes(value);
}

/**
 * @tparam IntType 
 * @param value 
 * @return constexpr int 
 */
template<Integral IntType>
inline constexpr int bit_scan_forward(IntType value)
{
#if defined(__GNUC__) || defined(__clang__)
    static_assert(sizeof(IntType) <= sizeof(unsigned long long));
    if constexpr (sizeof(IntType) <= sizeof(unsigned int))
        return __builtin_ffs(value);
    if constexpr (sizeof(IntType) == sizeof(unsigned long))
        return __builtin_ffsl(value);
    if constexpr (sizeof(IntType) == sizeof(unsigned long long))
        return __builtin_ffsll(value);
    VERIFY_NOT_REACHED();
#else
    if (value == 0)
        return 0;
    return 1 + count_trailing_zeroes(static_cast<MakeUnsigned<IntType>>(value));
#endif
}
