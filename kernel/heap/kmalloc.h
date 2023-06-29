/**
 * @file kmalloc.h
 * @author Krisna Pranav
 * @brief kmalloc
 * @version 1.0
 * @date 2023-06-29
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "types.h"

#define KMALLOC_SCRUB_BYTE 0xbb
#define KFREE_SCRUB_BYTE 0xaa

void kmalloc_init();
[[gnu::malloc, gnu::returns_nonnull, gnu::alloc_size(1)]] void* kmalloc_impl(size_t);
[[gnu::malloc, gnu::returns_nonnull, gnu::alloc_size(1)]] void* kmalloc_eternal(size_t);

/**
 * @brief realloc
 * 
 * @param size_t 
 * @return void* 
 */
void* krealloc(void* size_t);

/**
 * @brief free
 * 
 */
void kfree(void*);

struct kmalloc_stats {
    size_t bytes_allocated;
    size_t bytes_free;
    size_t bytes_eternal;
    size_t kfree_call_count;
};

void get_kamlloc_stats(kmalloc_stats&);

extern bool g_dump_kmalloc_stacks;

inline void* operator new(size_t, void* p) {
    return p;
}

inline void* operator new[](size_t, void* p) {
    return p;
}