/*
* Copyright (c) 2021, Krisna Pranav
*
* SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libutils/traits.h>
#include <stddef.h>

#if __CONFIG_IS_HOSTED__ == 0

inline void *operator new(size_t, void *ptr)
{
    return ptr;
}

#else
#   include <new>
#endif

namespace std
{

// FIXME: add more templates

using nullptr_t = decltype(nullptr);

}