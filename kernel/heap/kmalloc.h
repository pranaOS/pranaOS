/**
 * @file kmalloc.h
 * @author Krisna Pranav
 * @brief kmalloc
 * @version 6.0
 * @date 2023-06-29
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>

#define KMALLOC_SCRUB_BYTE 0xbb
#define KFREE_SCRUB_BYTE 0xaa

void kmalloc_init();

/**
 * @return void* 
 */
[[gnu::malloc, gnu::returns_nonnull, gnu::alloc_size(1)]] void* kmalloc_impl(size_t);

/**
 * @return void* 
 */
[[gnu::malloc, gnu::returns_nonnull, gnu::alloc_size(1)]] void* kmalloc_eternal(size_t);

/**
 * @return void* 
 */
void* krealloc(void*, size_t);

void kfree(void*);

struct kmalloc_stats {
    size_t bytes_allocated;
    size_t bytes_free;
    size_t bytes_eternal;
    size_t kmalloc_call_count;
    size_t kfree_call_count;
};

/**
 * @brief Get the kmalloc stats object
 * 
 */
void get_kmalloc_stats(kmalloc_stats&);

extern bool g_dump_kmalloc_stacks;

/**
 * @param p 
 * @return void* 
 */
inline void* operator new(size_t, void* p) { 
    return p; 
}

/**
 * @param p 
 * @return void* 
 */
inline void* operator new[](size_t, void* p) { 
    return p; 
}

/**
 * @param size 
 * @return ALWAYS_INLINE* 
 */
[[gnu::malloc, gnu::returns_nonnull, gnu::alloc_size(1)]] ALWAYS_INLINE void* kmalloc(size_t size) {
#ifdef KMALLOC_DEBUG_LARGE_ALLOCATIONS
    if (size >= 1048576)
        asm volatile("cli;hlt");
#endif
    return kmalloc_impl(size);
}

/**
 * @tparam ALIGNMENT 
 * @param size 
 * @return void* 
 */
template<size_t ALIGNMENT>
[[gnu::malloc, gnu::returns_nonnull, gnu::alloc_size(1)]] inline void* kmalloc_aligned(size_t size) {
    static_assert(ALIGNMENT > 1);
    static_assert(ALIGNMENT < 255);
    void* ptr = kmalloc(size + ALIGNMENT + sizeof(u8));
    size_t max_addr = (size_t)ptr + ALIGNMENT;
    void* aligned_ptr = (void*)(max_addr - (max_addr % ALIGNMENT));
    ((u8*)aligned_ptr)[-1] = (u8)((u8*)aligned_ptr - (u8*)ptr);
    return aligned_ptr;
}

/**
 * @param ptr 
 */
inline void kfree_aligned(void* ptr) {
    kfree((u8*)ptr - ((u8*)ptr)[-1]);
}

void kmalloc_enable_expand();
extern u8* const kmalloc_start;
extern u8* const kmalloc_end;
