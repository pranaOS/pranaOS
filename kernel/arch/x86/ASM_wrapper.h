/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <base/Types.h>

namespace Kernel {

ALWAYS_INLINE void cli()
{
    asm volatile("cli" ::
                     : "memory");
}
ALWAYS_INLINE void sti()
{
    asm volatile("sti" ::
                     : "memory");
}
ALWAYS_INLINE FlatPtr cpu_flags()
{
    FlatPtr flags;
    asm volatile(
        "pushf\n"
        "pop %0\n"
        : "=rm"(flags)::"memory");
    return flags;
}

#if ARCH(I386)
ALWAYS_INLINE void set_fs(u16 segment)
{
    asm volatile(
        "mov %%ax, %%fs" ::"a"(segment)
        : "memory");
}

ALWAYS_INLINE void set_gs(u16 segment)
{
    asm volatile(
        "mov %%ax, %%gs" ::"a"(segment)
        : "memory");
}

ALWAYS_INLINE u16 get_fs()
{
    u16 fs;
    asm("mov %%fs, %%eax"
        : "=a"(fs));
    return fs;
}

ALWAYS_INLINE u16 get_gs()
{
    u16 gs;
    asm("mov %%gs, %%eax"
        : "=a"(gs));
    return gs;
}
#endif

ALWAYS_INLINE FlatPtr read_gs_ptr(FlatPtr offset)
{
    FlatPtr val;
    asm volatile(
        "mov %%gs:%a[off], %[val]"
        : [val] "=r"(val)
        : [off] "ir"(offset));
    return val;
}

ALWAYS_INLINE void write_gs_ptr(u32 offset, FlatPtr val)
{
    asm volatile(
        "mov %[val], %%gs:%a[off]" ::[off] "ir"(offset), [val] "ir"(val)
        : "memory");
}