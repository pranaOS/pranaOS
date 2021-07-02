/*
 * Copyright (c) 2018-2020, krishpranav <krisna.pranav@gmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#ifndef __pranaos__
#    include <new>
#endif

#ifdef KERNEL
#    define AK_MAKE_ETERNAL                                               \
    public:                                                               \
        void* operator new(size_t size) { return kmalloc_eternal(size); } \
                                                                          \
    private:
#else
#    define AK_MAKE_ETERNAL
#endif

#if defined(KERNEL)
#    include <Kernel/Heap/kmalloc.h>
#else
#    include <stdlib.h>

#    define kcalloc calloc
#    define kmalloc malloc
#    define kfree free
#    define krealloc realloc

#    ifdef __pranaos__

#        include <new>

inline void* operator new(size_t size)
{
    return kmalloc(size);
}

inline void operator delete(void* ptr)
{
    return kfree(ptr);
}

inline void operator delete(void* ptr, size_t)
{
    return kfree(ptr);
}

inline void* operator new[](size_t size)
{
    return kmalloc(size);
}

inline void operator delete[](void* ptr)
{
    return kfree(ptr);
}

inline void operator delete[](void* ptr, size_t)
{
    return kfree(ptr);
}

#    endif

#endif
