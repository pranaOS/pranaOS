/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef _KERNEL_MEM_KMALLOC_H
#define _KERNEL_MEM_KMALLOC_H

#include <libkern/types.h>
#include <mem/vmm/vmm.h>

#define KMALLOC_SPACE_SIZE (4 * MB)
#define KMALLOC_BLOCK_SIZE 32
// #define KMALLOC_SCRUB_BYTE 0xbb
// #define KMALLOC_SCRUB_BYTE 0xaa

void kmalloc_init();
void* kmalloc(uint32_t size);
void* kmalloc_aligned(uint32_t size, uint32_t alignment);
void* kmalloc_page_aligned();
void kfree(void* ptr);
void kfree_aligned(void* ptr);
void kfree_sized(void*, size_t);
void* krealloc(void* ptr, uint32_t size);

void kmalloc_enabled_expand();

#endif // _KERNEL_MEM_KMALLOC_H
