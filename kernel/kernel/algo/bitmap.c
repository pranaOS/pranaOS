/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <algo/bitmap.h>
#include <libkernel/bits/errno.h>
#include <mem/kmalloc.h>

#define BITMAP_BLOCKS_PER_BYTE (8)

bitmap_t bitmap_wrap(uint8_t* data, uint32_t len)
{
    bitmap_t bitmap;
    bitmap.data = data;
    bitmap.len = len;
    return bitmap;
}

int bitmap_unset_range(bitmap_t bitmap, int start, int len)
{
    if (start + len - 1 >= bitmap.len * BITMAP_BLOCKS_PER_BYTE) {
        return -EFAULT;
    }

    int where = start;

    while (len && (where % BITMAP_BLOCKS_PER_BYTE) != 0) {
        bitmap_unset(bitmap, where);
        where++;
        len--;
    }

    while (len >= BITMAP_BLOCKS_PER_BYTE) {
        int block = where / BITMAP_BLOCKS_PER_BYTE;
        bitmap.data[block] = 0x0;
        where += BITMAP_BLOCKS_PER_BYTE;
        len -= BITMAP_BLOCKS_PER_BYTE;
    }

    while (len) {
        bitmap_unset(bitmap, where);
        where++;
        len--;
    }

    return 0;
}