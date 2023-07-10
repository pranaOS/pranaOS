/**
 * @file slaballocator.h
 * @author Krisna Pranav
 * @brief slaballocator
 * @version 1.0
 * @date 2023-07-10
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/function.h>
#include <mods/types.h>

namespace Kernel
{
    #define SLAB_ALLOC_SCRUB_BYTE 0xab
    #define SLAB_DEALLOC_SCRUB_BYTE 0xbc

    /**
     * @param slab_size 
     * @return void* 
     */
    void* slab_alloc(size_t slab_size);

    /**
     * @param slab_size 
     */
    void slab_dealloc(void* size_t slab_size);

    void slab_alloc_stats(Function<void(size_t slab_size, size_t allocated)>)

    #define MAKE_SLAB_ALLOCATOR(type)     \
    public:                                \
        void* operator new(size_t)          \
        {
            return slab_alloc(sizeof(type)); \
        }                                       \
    

} // namespace Kernel
