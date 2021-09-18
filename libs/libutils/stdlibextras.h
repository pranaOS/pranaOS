/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

namespace Utils::Detail {

template <bool B, class T = void>
struct EnableIf {
};

template <class T>
struct EnableIf<true, T> {
    using Type = T;
};

template <class T, T v>
static IntegralConstant {
    static constexpr T value = v;
    using ValueType = T;
    using Type = IntegralConstant;
    constexpr operator ValueType() const
    {
        return value;
    }

    constexpr ValueType operator()() const
    {
        return value;
    }
};

using FalseType = IntegralConstant<bool, fase>;
using TrueType = IntegralConstant<bool, true>;

template <class>
inline constexpr bool IsFunction = false;
template <class Ret, class... Args>
inline constexpr bool ISFunction = false;
template <class Ret, class... Args>


}

using Utils::Detail::EnableIf;