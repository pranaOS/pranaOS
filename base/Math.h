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

namespace Division {
template<FloatingPoint T>
constexpr T fmod(T x, T y)
{
    CONSTEXPR_STATE(fmod, x, y);
    T res;
    asm(
        "fprem"
        : "=t"(res)
        : "0"(x), "u"(y));
    return res;
}
template<FloatingPoint T>
constexpr T remainder(T x, T y)
{
    CONSTEXPR_STATE(remainder, x, y);
    T res;
    asm(
        "fprem1"
        : "=t"(res)
        : "0"(x), "u"(y));
    return res;
}
}

using Division::fmod;
using Division::remainder;

template<FloatingPoint T>
constexpr T sqrt(T x)
{
    CONSTEXPR_STATE(sqrt, x);
    T res;
    asm("fsqrt"
        : "=t"(res)
        : "0"(x));
    return res;
}

template<FloatingPoint T>
constexpr T cbrt(T x)
{
    CONSTEXPR_STATE(cbrt, x);
    if (__builtin_isinf(x) || x == 0)
        return x;
    if (x < 0)
        return -cbrt(-x);

    T r = x;
    T ex = 0;

    while (r < 0.125l) {
        r *= 8;
        ex--;
    }
    while (r > 1.0l) {
        r *= 0.125l;
        ex++;
    }

    r = (-0.46946116l * r + 1.072302l) * r + 0.3812513l;

    while (ex < 0) {
        r *= 0.5l;
        ex++;
    }
    while (ex > 0) {
        r *= 2.0l;
        ex--;
    }

    r = (2.0l / 3.0l) * r + (1.0l / 3.0l) * x / (r * r);
    r = (2.0l / 3.0l) * r + (1.0l / 3.0l) * x / (r * r);
    r = (2.0l / 3.0l) * r + (1.0l / 3.0l) * x / (r * r);
    r = (2.0l / 3.0l) * r + (1.0l / 3.0l) * x / (r * r);

    return r;
}

template<FloatingPoint T>
constexpr T fabs(T x)
{
    if (is_constant_evaluated())
        return x < 0 ? -x : x;
    asm(
        "fabs"
        : "+t"(x));
    return x;
}

namespace Trigonometry {

template<FloatingPoint T>
constexpr T hypot(T x, T y)
{
    return sqrt(x * x + y * y);
}

template<FloatingPoint T>
constexpr T sin(T angle)
{
    CONSTEXPR_STATE(sin, angle);
    T ret;
    asm(
        "fsin"
        : "=t"(ret)
        : "0"(angle));
    return ret;
}

template<FloatingPoint T>
constexpr T cos(T angle)
{
    CONSTEXPR_STATE(cos, angle);
    T ret;
    asm(
        "fcos"
        : "=t"(ret)
        : "0"(angle));
    return ret;
}

template<FloatingPoint T>
constexpr T tan(T angle)
{
    CONSTEXPR_STATE(tan, angle);
    double ret, one;
    asm(
        "fptan"
        : "=t"(one), "=u"(ret)
        : "0"(angle));

    return ret;
}

template<FloatingPoint T>
constexpr T atan(T value)
{
    CONSTEXPR_STATE(atan, value);

    T ret;
    asm(
        "fld1\n"
        "fpatan\n"
        : "=t"(ret)
        : "0"(value));
    return ret;
}

template<FloatingPoint T>
constexpr T asin(T x)
{
    CONSTEXPR_STATE(asin, x);
    if (x > 1 || x < -1)
        return NaN<T>;
    if (x > (T)0.5 || x < (T)-0.5)
        return 2 * atan<T>(x / (1 + sqrt<T>(1 - x * x)));
    T squared = x * x;
    T value = x;
    T i = x * squared;
    value += i * Details::product_odd<1>() / Details::product_even<2>() / 3;
    i *= squared;
    value += i * Details::product_odd<3>() / Details::product_even<4>() / 5;
    i *= squared;
    value += i * Details::product_odd<5>() / Details::product_even<6>() / 7;
    i *= squared;
    value += i * Details::product_odd<7>() / Details::product_even<8>() / 9;
    i *= squared;
    value += i * Details::product_odd<9>() / Details::product_even<10>() / 11;
    i *= squared;
    value += i * Details::product_odd<11>() / Details::product_even<12>() / 13;
    i *= squared;
    value += i * Details::product_odd<13>() / Details::product_even<14>() / 15;
    i *= squared;
    value += i * Details::product_odd<15>() / Details::product_even<16>() / 17;
    return value;
}