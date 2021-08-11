/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <base/Checked.h>

#if defined(KERNEL)
#    include <Kernel/Heap/kmalloc.h>
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

#ifndef __pranaos__
#    include <base/Types.h>

#    ifndef BASE_OS_MACOS
extern "C" {
inline size_t malloc_good_size(size_t size) { return size; }
}
#    else
#        include <malloc/malloc.h>
#    endif
#endif

#ifdef KERNEL
#    define BASE_MAKE_ETERNAL                                               \
    public:                                                               \
        void* operator new(size_t size) { return kmalloc_eternal(size); } \
                                                                          \
    private:
#else
#    define BASE_MAKE_ETERNAL
#endif

using std::nothrow;

inline void* kmalloc_array(Checked<size_t> a, Checked<size_t> b)
{
    auto size = a * b;
    VERIFY(!size.has_overflow());
    return kmalloc(size.value());
}

inline void* kmalloc_array(Checked<size_t> a, Checked<size_t> b, Checked<size_t> c)
{
    auto size = a * b * c;
    VERIFY(!size.has_overflow());
    return kmalloc(size.value());
}
