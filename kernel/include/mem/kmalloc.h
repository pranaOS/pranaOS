/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libkern/types.h>
#include <mem/vmm/vmm.h>

#define KMALLOC_SPACE_SIZE (4 * MB)
#define KMALLOC_BLOCK_SIZE 32

void kmalloc_init();
void* kmalloc(uint32_t size);
void* kmalloc_aligned(uint32_t size, uint32_t alignment);
void* kmalloc_page_aligned();
void kfree(void* ptr);
void kfree_aligned(void* ptr);
void kfree_sized(void*, size_t);
void* krealloc(void* ptr, uint32_t size);
void kmalloc_enabled_expand();

size_t kmalloc_good_size(size_t);

struct kmalloc_stats {
    size_t bytes_allocated;
    size_t bytes_free;
    size_t bytes_eternal;
    size_t kmallc_call_count;
    size_t kfree_call_count;
};
