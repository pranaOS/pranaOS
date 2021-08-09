/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <algo/bitmap.h>
#include <libkernel/libkernel.h>
#include <libkernel/lock.h>
#include <libkernel/log.h>
#include <mem/kmalloc.h>
#include <mem/vmm/zoner.h>

struct kmalloc_header {
    uint32_t len;
};
typedef struct kmalloc_header kmalloc_header_t;

static lock_t _kmalloc_lock;
static zone_t _kmalloc_zone;
static uint32_t _kmalloc_bitmap_len = 0;
static uint8_t* _kmalloc_bitmap;
static bitmap_t bitmap;

static inline uint32_t kmalloc_to_vaddr(int start)
{
    uint32_t vaddr = (uint32_t)_kmalloc_zone.start + start * KMALLOC_BLOCK_SIZE;
    return (uint32_t)_kmalloc_zone.start + start * KMALLOC_BLOCK_SIZE;
}

static inline int kmalloc_to_index(uint32_t vaddr)
{
    return (vaddr - (uint32_t)_kmalloc_zone.start) / KMALLOC_BLOCK_SIZE;
}

static void _kmalloc_init_bitmap()
{
    _kmalloc_bitmap = (uint8_t*)_kmalloc_zone.start;
    _kmalloc_bitmap_len = (KMALLOC_SPACE_SIZE / KMALLOC_BLOCK_SIZE / 8);

    bitmap = bitmap_wrap(_kmalloc_bitmap, _kmalloc_bitmap_len);
    memset(_kmalloc_bitmap, 0, _kmalloc_bitmap_len);

    int blocks_needed = (_kmalloc_bitmap_len + KMALLOC_BLOCK_SIZE - 1) / KMALLOC_BLOCK_SIZE;
    bitmap_set_range(bitmap, kmalloc_to_index((uint32_t)_kmalloc_bitmap), blocks_needed);
}

void kmalloc_init()
{
    lock_init(&_kmalloc_lock);
    _kmalloc_zone = zoner_new_zone(KMALLOC_SIZE);
    _kmalloc_init_bitmap();
}