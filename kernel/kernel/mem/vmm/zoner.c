/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <algo/bitmap.h>
#include <libkernel/bits/errno.h>
#include <libkernel/libkernel.h>
#include <libkernel/lock.h>
#include <mem/vmm/vmm.h>
#include <mem/vmm/zoner.h>

#define ZONER_BITMAP_SIZE (4 * 1024 * 8)
#define ZONER_TO_BITMAP_INDEX(x) ((x - KERNEL_BASE) >> 12)
#define ZONER_FROM_BITMAP_INDEX(x) ((x << 12) + KERNEL_BASE)

static uint32_t _zoner_next_vaddr;
static uint8_t* _zoner_bitmap;
static lock_t _zoner_lock;
static bitmap_t bitmap;
static bool _zoner_bitmap_set;

static uint32_t _zoner_new_vzone_lockless(uint32_t size)
{
    uint32_t res = _zoner_next_vaddr;
    _zoner_next_vaddr += size;
    return res;
}

static uint32_t _zoner_new_vzone_aligned_lockless(uint32_t size, uint32_t alignment)
{
    uint32_t res = _zoner_next_vaddr;
    _zoner_next_vaddr += size + alignment;
    res += alignment;
    res = res - (res % alignment);
    return res;
}

void zoner_place_bitmap()
{
    lock_acquire(&_zoner_lock);
    _zoner_bitmap = (uint8_t*)_zoner_new_vzone_lockless(ZONER_BITMAP_SIZE);
    bitmap = bitmap_wrap(_zoner_bitmap, ZONER_BITMAP_SIZE * 8);
    memset(_zoner_bitmap, 0, ZONER_BITMAP_SIZE);
    _zoner_bitmap_set = true;
    bitmap_set_range(bitmap, 0, ZONER_TO_BITMAP_INDEX(_zoner_next_vaddr));
    lock_release(&_zoner_lock);
}

void zoner_init(uint32_t start_vaddr)
{
    lock_init(&_zoner_lock);
    _zoner_next_vaddr = start_vaddr;
}