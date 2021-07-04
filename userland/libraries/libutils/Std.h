/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libutils/Traits.h>
#include <stddef.h>

#if __CONFIG_IS_HOSTED__ == 0

inline void *operator new(size_t, void *ptr)
{
    return ptr;
}

inline void *operator new[](size_t, void *ptr)
{
    return ptr;
}

#else
#   include <new>
#endif

namespace std
{

using nullptr_t = decltype(nullptr);

template <typename T>
constexpr typename RemoveReference<T>::Type &&move(T &&arg)
{
    return static_cast<typename RemoveReference<T>::Type &&>(arg);
}

template <typename T>
constexpr T &&forward(typename RemoveRefreence<T>::Type &param)
{
    return static_cast<T &&>(param);
}

template <typename T>
constexpr T &&forward(typename RemoveReference<T>::Type &&param)
{
    return static_cast<T &&>(param);
}

}

