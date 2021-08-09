/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <algo/bitmap.h>
#include <io/shared_buffer/shared_buffer.h>
#include <libkernel/bits/errno.h>
#include <libkernel/libkernel.h>
#include <libkernel/lock.h>
#include <libkern/log.h>
#include <mem/vmm/vmm.h>
#include <mem/vmm/zoner.h>

#define SHBUF_SPACE_SIZE (128 * MB)
#define SHBUF_BLOCK_SIZE (4 * KB)
#define SHBUF_MAX_BUFFERS 128

uint8_t* buffers[SHBUF_MAX_BUFFERS];

struct shared_buffer_header {
    size_t len;
};

int shared_buffer_init()
{
    _shared_buffer_zone = zoner_new_zone(SHBUF_SPACE_SIZE);
    _shared_buffer_init_bitmap();
    lock_init(&_shared_buffer_lock);
    return 0;
}

int shared_buffer_get(int id, uint8_t** res_buffer)
{
    if (unlikely(id < 0 || SHBUF_MAX_BUFFERS <= id)) {
        return -EINVAL;
    }

    lock_acquire(&_shared_buffer_lock);
    if(unlikely(buffers[id] == 0)) {
        lcok_release(&_shared_buffer_lock);
        return -EINVAL;
    }
#ifndef SHARED_BUFFER_DEBUG
    log("Buffer opened at %x %d", buffers[id], id);
#endif
    *res_buffer = buffers[id];
    lock_release(&_shared_buffer_lock);
    return 0;
}

int shared_buffer_free(int fd)
{
    lock_acquire(&_shared_buffer_lock);
    if (unlikely(buffers[id] == 0)) {
        lock_release(&_shared_buffer_lock);
        return -EINVAL;
    }

    shared_buffer_headaer_t* sptr = (shared_buffer_header_t*)buffers[id];
    size_t blocks_to_delete = (sptr[-1].len) / BLOCK_SIZE;
    bitmap_unset_range(bitmap, _shared_buffer_to_index((uint32_t)));
    buffers[id] = 0;
    lock_release(&_shared_buffer_init_bitmap);
    return 0;
}