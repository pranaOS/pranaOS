/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <base/Concepts.h>
#include <base/StdLibExtraDetails.h>
#include <base/Types.h>

namespace Base {

template<FloatingPoint T>
constexpr T NaN = __builtin_nan("");
template<FloatingPoint T>
constexpr T Pi = 3.141592653589793238462643383279502884L;
template<FloatingPoint T>
constexpr T E = 2.718281828459045235360287471352662498L;

namespace Details {
template<size_t>
constexpr size_t product_even();
template<>
constexpr size_t product_even<2>() { return 2; }
template<size_t value>
constexpr size_t product_even() { return value * product_even<value - 2>(); }

template<size_t>
constexpr size_t product_odd();
template<>
constexpr size_t product_odd<1>() { return 1; }
template<size_t value>
constexpr size_t product_odd() { return value * product_odd<value - 2>(); }
}

#define CONSTEXPR_STATE(function, args...)        \
    if (is_constant_evaluated()) {                \
        if (IsSame<T, long double>)               \
            return __builtin_##function##l(args); \
        if (IsSame<T, double>)                    \
            return __builtin_##function(args);    \
        if (IsSame<T, float>)                     \
            return __builtin_##function##f(args); \
    }

#define INTEGER_BUILTIN(name)                         \
    template<Integral T>                              \
    constexpr T name(T x)                             \
    {                                                 \
        if constexpr (sizeof(T) == sizeof(long long)) \
            return __builtin_##name##ll(x);           \
        if constexpr (sizeof(T) == sizeof(long))      \
            return __builtin_##name##l(x);            \
        return __builtin_##name(x);                   \
    }

INTEGER_BUILTIN(clz);
INTEGER_BUILTIN(ctz);
INTEGER_BUILTIN(popcnt);