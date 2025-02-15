/**
 * @file stdlibextras.h
 * @author Krisna Pranav
 * @brief StdLIBExtra.h
 * @version 6.0
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/stdlibextradetail.h>
#include <mods/assertions.h>

/**
 * @tparam T 
 * @tparam U 
 */
template <typename T, typename U>
constexpr auto round_up_to_power_of_two(T value, U power_of_two)
    requires(IsIntegral<T> && IsIntegral<U>)
{
    return ((value - 1) & ~(power_of_two - 1)) + power_of_two;
}

/**
 * @tparam T 
 */
template <typename T>
constexpr bool is_power_of_two(T value)
    requires(IsIntegral<T>)
{
    return value && !((value) & (value - 1));
}

#ifndef MODS_DONT_REPLACE_STD
namespace std
{
    /**
     * @tparam T 
     * @param param 
     * @return constexpr T&& 
     */
    template <typename T>
    constexpr T&& forward(Mods::Detail::RemoveReference<T>& param)
    {
        return static_cast<T&&>(param);
    }   

    /**
     * @tparam T 
     * @param param 
     * @return constexpr T&& 
     */
    template <typename T>
    constexpr T&& forward(Mods::Detail::RemoveReference<T>&& param) noexcept
    {
        static_assert(!IsLvalueReference<T>, "Can't forward an rvalue as an lvalue.");
        return static_cast<T&&>(param);
    }

    /**
     * @tparam T 
     * @param arg 
     * @return constexpr T&& 
     */
    template <typename T>
    constexpr T&& move(T& arg)
    {
        return static_cast<T&&>(arg);
    }

} // namespace std
#else
#include <utility>
#endif

using std::forward;
using std::move;

namespace Mods::Detail
{
    /**
     * @tparam T 
     */
    template <typename T>
    struct _RawPtr
    {
        using Type = T*;
    };
} // namespace Mods::Detail

namespace Mods
{

/**
 * @tparam T 
 * @tparam SizeType 
 * @tparam N 
 * @return constexpr SizeType 
 */
template <typename T, typename SizeType = decltype(sizeof(T)), SizeType N>
constexpr SizeType array_size(T (&)[N])
{
    return N;
}

/**
 * @tparam T 
 * @param a 
 * @param b 
 * @return constexpr T 
 */
template <typename T>
constexpr T min(const T& a, IdentityType<T> const& b)
{
    return b < a ? b : a;
}

/**
 * @tparam T 
 * @param a 
 * @param b 
 * @return constexpr T 
 */
template <typename T>
constexpr T max(const T& a, IdentityType<T> const& b)
{
    return a < b ? b : a;
}

/**
 * @tparam T 
 * @param value 
 * @param min 
 * @param max 
 * @return constexpr T 
 */
template <typename T>
constexpr T clamp(const T& value, IdentityType<T> const& min, IdentityType<T> const& max)
{
    VERIFY(max >= min);
    if(value > max)
        return max;
    if(value < min)
        return min;
    return value;
}

/**
 * @tparam T 
 * @tparam U 
 * @param v1 
 * @param v2 
 * @param interpolation 
 * @return constexpr T 
 */
template <typename T, typename U>
constexpr T mix(T const& v1, T const& v2, U const& interpolation)
{
    return v1 + (v2 - v1) * interpolation;
}

/**
 * @tparam T 
 * @tparam U 
 * @param a 
 * @param b 
 * @return constexpr T 
 */
template <typename T, typename U>
constexpr T ceil_div(T a, U b)
{
    static_assert(sizeof(T) == sizeof(U));
    T result = a / b;
    if((a % b) != 0)
        ++result;
    return result;
}

/**
 * @tparam T 
 * @tparam U 
 * @param a 
 * @param b 
 */
template <typename T, typename U>
inline void swap(T& a, U& b)
{
    if(&a == &b)
        return;
    U tmp = move((U&)a);
    a = (T&&)move(b);
    b = move(tmp);
}

/**
 * @tparam T 
 * @tparam U 
 * @param slot 
 * @param value 
 * @return constexpr T 
 */
template <typename T, typename U = T>
constexpr T exchange(T& slot, U&& value)
{
    T old_value = move(slot);
    slot = forward<U>(value);
    return old_value;
}

/**
 * @tparam T 
 */
template <typename T>
using RawPtr = typename Detail::_RawPtr<T>::Type;

/**
 * @tparam V 
 */
template <typename V>
constexpr decltype(auto) to_underlying(V value)
    requires(IsEnum<V>)
{
    return static_cast<UnderlyingType<V>>(value);
}

/**
 * @return true 
 * @return false 
 */
constexpr bool is_constant_evaluated()
{
#if __has_builtin(__builtin_is_constant_evaluated)
    return __builtin_is_constant_evaluated();
#else
    return false;
#endif
}

#define __DEFINE_GENERIC_ABS(type, zero, intrinsic) \
    constexpr type abs(type num)                    \
    {                                               \
        if(is_constant_evaluated())                 \
            return num < (zero) ? -num : num;       \
        return __builtin_##intrinsic(num);          \
    }

__DEFINE_GENERIC_ABS(int, 0, abs);
__DEFINE_GENERIC_ABS(long, 0L, labs);
__DEFINE_GENERIC_ABS(long long, 0LL, llabs);
#ifndef KERNEL
__DEFINE_GENERIC_ABS(float, 0.0F, fabsf);
__DEFINE_GENERIC_ABS(double, 0.0, fabs);
__DEFINE_GENERIC_ABS(long double, 0.0L, fabsl);
#endif

#undef __DEFINE_GENERIC_ABS

} // namespace Mods

using Mods::array_size;
using Mods::ceil_div;
using Mods::clamp;
using Mods::exchange;
using Mods::is_constant_evaluated;
using Mods::max;
using Mods::min;
using Mods::mix;
using Mods::RawPtr;
using Mods::swap;
using Mods::to_underlying;