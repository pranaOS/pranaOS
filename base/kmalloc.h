/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#if defined(KERNEL)
#    include <kernel/Heap/kmalloc.h>
#else
#    include <new>
#    include <stdlib.h>

#    define kcalloc calloc
#    define kmalloc malloc
#    define kmalloc_good_size malloc_good_size
#    define kfree free

inline void kfree_sized(void* ptr, size_t)
{
    kfree(ptr);
}
#endif

#ifndef __serenity__
#    include <base/Types.h>

#    ifndef BASE_OS_MACOS
extern "C" {
inline size_t malloc_good_size(size_t size) { return size; }
}
#    else
#        include <malloc/malloc.h>
#    endif
#endif