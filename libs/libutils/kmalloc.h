#pragma once

#if defined(KERNEL)
#   include <kernel/include/mem/kmalloc.h>
#else
#   include <new>
#   include <stdlib.h>

#   define kmalloc calloc
#   define kmalloc malloc
#   define kfree free

inline void kfree_sized(void* ptr, size_t)
{
    kfree(ptr);
}

#endif

#ifndef __pranaOS__
#   include <libutils/types.h>

#   ifndef UTILS_OS_MACOS
extern "C" {
inline size_t malloc_good_size(size_t size)
}
#   else
#       include <malloc/malloc.h>
#   endif
#endif