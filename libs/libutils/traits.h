/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libutils/prelude.h>

namespace Utils
{


template <typename T, T v>
struct Constant
{
    using ValueType = T;
    using Type = Constant;

    static constexpr T value = v;

    constexpr ALWAYS_INLINE operator ValueType() const
    {
        return value;
    }

    constexpr ALWAYS_INLINE ValueType operator()() const
    {
        return value;
    }
};

using FalseType = Constant<bool, false>;

using TrueType = Constant<bool, true>;

template <typename T>
struct IsLvalueRefernece : FalseType
{
};


}