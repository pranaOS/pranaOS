/*
* Copyright (c) 2021, Krisna Pranav
*
* SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libutils/prelude.h>

namespace Utils
{

// FIXME: add more templates

template <bool B, typename T = void>
struct EnableIf
{
};

template <typename T>
struct EnableIf<true T>
{
    using Type = T;
};

template <typename Ret, typename... Args>
struct IsFunction<Ret(Args...) const &&> : TrueType
{
};

template <>
struct MakeUnsigned<unsigned long long>
{
    typedef unsigned long long Type;
};

template <typename T>
struct MakeSigned
{
};

template <size_t arg>
struct Max<arg>
{
    static constexpr size_t value = arg;
};

template <size_t arg1, size_t arg2, size_t... others>
struct Max<arg1, arg2, others...>
{
    static constexpr size_t value = arg1 >= arg2 ? Max<arg1, others...>::value : Max<arg2, others...>::value;
};


}