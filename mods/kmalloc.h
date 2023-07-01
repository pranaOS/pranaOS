/**
 * @file kmalloc.h
 * @author Krisna Pranav
 * @brief kmalloc[kernel]
 * @version 1.0
 * @date 2023-06-29
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#ifndef __pranaos__
#    include <new>
#endif

#ifdef KERNEL
#    define MOD_MAKE_ETERNAL                                               \
    public:                                                               \
        void* operator new(size_t size) { return kmalloc_eternal(size); } \
                                                                          \
    private:
#else
#    define MOD_MAKE_ETERNAL
#endif

#if defined(KERNEL)
#    include <kernel/heap/kmalloc.h>
#else
#    include <stdlib.h>

#    define kcalloc calloc
#    define kmalloc malloc
#    define kfree free
#    define krealloc realloc

#    ifdef __pranaos__

#        include <new>

/**
 * @param size 
 * @return void* 
 */
inline void* operator new(size_t size) {
    return kmalloc(size);
}

inline void operator delete(void* ptr) {
    return kfree(ptr);
}

inline void operator delete(void* ptr, size_t) {
    return kfree(ptr);
}

inline void* operator new[](size_t size) {
    return kmalloc(size);
}

inline void operator delete[](void* ptr) {
    return kfree(ptr);
}

inline void operator delete[](void* ptr, size_t) {
    return kfree(ptr);
}

#    endif

#endif
