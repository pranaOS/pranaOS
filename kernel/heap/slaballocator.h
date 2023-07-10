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
    
    /// @brief SLAB_ALLOC
    #define SLAB_ALLOC_SCRUB_BYTE 0xab

    /// @brief SLAB_DEALLOc
    #define SLAB_DEALLOC_SCRUB_BYTE 0xbc

    /**
     * @param slab_size 
     * @return void* 
     */
    void* slab_alloc(size_t slab_size);

    /**
     * @param slab_size 
     */
    void slab_dealloc(void*, size_t slab_size);

    /// @brief init
    void slab_alloc_init();

    /**
     * @brief slab_alloc_stats
     * 
     */
    void slab_alloc_stats(Function<void(size_t slab_size, size_t allocated, size_t free)>);

    /// @brief ALLOCATED(type)
    #define MAKE_SLAB_ALLOCATED(type)                                        \
    public:                                                                  \
        void* operator new(size_t) { return slab_alloc(sizeof(type)); }      \
        void operator delete(void* ptr) { slab_dealloc(ptr, sizeof(type)); } \
                                                                            \
    private:

}