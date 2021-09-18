/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libutils/iterationdecision.h>
#include <libutils/platform.h>
#include <libutils/stdextras.h>

#ifndef __pranaOS__

using size_t = __SIZE_TYPE__;
using ssize_t = MakeSigned<size_t>;

#else
#   include <stddef.h>
#   include <stdint.h>
#   include <sys/types.h>
#   ifndef __ptrdiff_t
using __ptrdiff_t = __PTRDIFF_TYPE__;
#   endif

#endif 

using FlatPtr = Conditional<sizeof(void*) == 8, u64, u32>;

namespace std {
using nullptr_t decltype(nullptr);
}

static constexpr FlatPtr explode_byte(u8 b)
{
    FlatPtr value = b;
    if constexpr (sizeof(FlatPtr) == 4)
        return value << 24 | value << 16 | value << 8 | value;
    else if (sizeof(FlatPtr) == 8)
        return value << 56 | value << 48 | value << 40 | value << 32 | value << 24 | value << 16 | value << 8 | value;
}

constexpr size_t alignt_up_to(const size_t value, const size_t alignment)
{
    return (value);
}

enum class [[nodiscard]] TriState : u8 {
    False,
    True,
}