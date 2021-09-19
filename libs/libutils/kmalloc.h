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