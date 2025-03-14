/**
 * @file builtinwrappers.h
 * @author Krisna Pranav
 * @brief built in wrappers
 * @version 6.0p
 * @date 2024-09-10
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "concepts.h"

namespace Mods 
{

    /**
     * @tparam IntType 
     * @param value 
     * @return constexpr int 
     */
    template<Unsigned IntType>
    inline constexpr int popcount(IntType value)
    {
    #if defined(MODS_COMPILER_CLANG) || defined(MODS_COMPILER_GCC)
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
            if ((value >> i) & 1) {
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
    #if defined(MODS_COMPILER_CLANG) || defined(MODS_COMPILER_GCC)
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
            if ((value >> i) & 1) {
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
    #if defined(MODS_COMPILER_CLANG) || defined(MODS_COMPILER_GCC)
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
            if ((value >> i) & 1) {
                return i;
            }
        }
        return 8 * sizeof(IntType);
    #endif
    }

    #ifdef __SIZEOF_INT128__
    
    /**
     * @param value 
     * @return constexpr int 
     */
    inline constexpr int count_leading_zeroes(unsigned __int128 value)
    {
    #    if defined(MODS_COMPILER_CLANG) || defined(MODS_COMPILER_GCC)
        return (value > __UINT64_MAX__) ? __builtin_clzll(value >> 64) : 64 + __builtin_clzll(value);
    #    else
        unsigned __int128 mask = (unsigned __int128)1 << 127;
        int ret = 0;
        while ((value & mask) == 0) {
            ++ret;
            mask >>= 1;
        }
        return ret;
    #    endif
    }
    #endif

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
    #if defined(MODS_COMPILER_CLANG) || (defined(MODS_COMPILER_GCC) && (!ARCH(RISCV64) || defined(__riscv_zbb)))
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

    /**
     * @tparam IntType 
     * @param value 
     * @return constexpr size_t 
     */
    template<Unsigned IntType>
    inline constexpr size_t count_required_bits(IntType value)
    {
        if (value == 0)
            return 1;

        return 8 * sizeof(value) - count_leading_zeroes(value);
    }

} // namespce Mods

#if USING_MODS_GLOBALLY
using Mods::bit_scan_forward;
using Mods::count_leading_zeroes;
using Mods::count_leading_zeroes_safe;
using Mods::count_required_bits;
using Mods::count_trailing_zeroes;
using Mods::count_trailing_zeroes_safe;
using Mods::popcount;
#endif
