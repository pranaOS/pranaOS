/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <base/Types.h>

#if defined(KERNEL)
#    include <kernel/StdLib.h>
#else
#    include <stdlib.h>
#    include <string.h>
#endif


ALWAYS_INLINE void fast_u32_copy(u32* dest, const u32* src, size_t count)
{
#if ARCH(I386)
    asm volatile(
        "rep movsl\n"
        : "+S"(src), "+D"(dest), "+c"(count)::"memory");
#else
    __builtin_memcpy(dest, src, count * 4);
#endif
}
