/**
 * @file kmalloc.h
 * @author Krisna Pranav
 * @brief kmalloc[kernel]
 * @version 6.0
 * @date 2023-06-29
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */


#pragma once

#include <mods/check.h>

#if defined(KERNEL)
#    include <kernel/heap/kmalloc.h>
#else
#    include <new>
#    include <stdlib.h>

#    define kmalloc malloc
#    define kmalloc_good_size malloc_good_size

/**
 * @param ptr 
 */
inline void kfree_sized(void* ptr, size_t)
{
    free(ptr);
}
#endif

#ifndef __pranaos__
#    include <mods/types.h>

#    ifndef MODS_OS_MACOS
extern "C" 
{
    /**
     * @param size 
     * @return size_t 
     */
    inline size_t malloc_good_size(size_t size) 
    { 
        return size; 
    }
}
#    else
#        include <malloc/malloc.h>
#    endif
#endif

using std::nothrow;

/**
 * @param a 
 * @param b 
 * @return void* 
 */
inline void* kmalloc_array(Checked<size_t> a, Checked<size_t> b)
{
    auto size = a * b;
    VERIFY(!size.has_overflow());
    return kmalloc(size.value());
}

/**
 * @param a 
 * @param b 
 * @param c 
 * @return void* 
 */
inline void* kmalloc_array(Checked<size_t> a, Checked<size_t> b, Checked<size_t> c)
{
    auto size = a * b * c;
    VERIFY(!size.has_overflow());
    return kmalloc(size.value());
}
