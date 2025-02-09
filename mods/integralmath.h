/**
 * @file integralmath.h
 * @author Krisna Pranav
 * @brief Integral Math
 * @version 6.0
 * @date 2023-07-20
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/builtinwrappers.h>
#include <mods/concept.h>
#include <mods/types.h>

namespace Mods
{
    /**
     * @tparam T 
     * @param exponent 
     * @return constexpr T 
     */
    template <Integral T>
    constexpr T exp2(T exponent)
    {
        return 1u << exponent;
    }

    /**
     * @tparam T 
     * @param x 
     * @return constexpr T 
     */
    template <Integral T>
    constexpr T log2(T x)
    {
        return x ? (8 * sizeof(T) - 1) - count_leading_zeroes(static_cast<MakeUnsigned<T>>(x)) : 0;
    }

    /**
     * @tparam I 
     * @param base 
     * @param exponent 
     * @return constexpr I 
     */
    template <Integral I>
    constexpr I pow(I base, I exponent)
    {
        if(exponent < 0)
            return 0;

        if(exponent == 0)
            return 1;

        I res = 1;

        while(exponent > 0)
        {
            if(exponent & 1)
                res *= base;
            base *= base;
            exponent /= 2u;
        }
        return res;
    }
} // namespace Mods