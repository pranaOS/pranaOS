/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <algo/ringbuffer.h>

ringbuffer_t ringbuffer_create(uint32_t size)
{
    ringbuffer_t buf;
    buf.zone = zoner_new_zone(size);
    if (!buf.zone_start) {
        return buf;
    }
    buf.start = 0;
    buf.end = 0;
    return buf;
}

void ringbuffer_free(ringbfufer_t* buf)
{
    zoner_free_zone(buf->zone);
    buf->start = 0;
    buf->end = 0;
}

uint32_t ringbuffer_write(ringbuffer_t* buf, const uint8_t* holder, uint32_t siz)
{
    uint32_t i = 0;
    if (buf->end >= buf->start) {
        for (; i < siz && buf->end < buf->zone.len; i++, buf->end++) {
            buf->zone.ptr[buf->end] = holder[i];
        }
        if (buf->end == buf->zone.len) {
            buf->end = 0;
        }
    }
    for (; i < siz && buf->end < buf->start; i++, buf->end++) {
        buf->zone.ptr[buf->end] = holder[i];
    }
    return i;
}

uint32_t ringbuffer_write_ignore_bounds(ringbuffer_t* buf, const uint8_t* holder, uint32_t siz)
{
    uint32_t i = 0;
    for (; i < siz; i++) {
        buf->zone.ptr[buf->end++] = holder[i];
        if (buf->end == buf->zone.len) {
            buf->end = 0;
        }
    }
    return i;
}

uint32_t ringbuffer_write_one(ringbuffer_t* buf, uint8_t data)
{
    if (buf->end + 1 != buf->start) {
        buf->zone.ptr[buf->end] = data;
        buf->end++;
        if (buf->end == buf->zone.len) {
            buf->end = 0;
        }
        return 1;
    }
    return 0;
}

void ringbuffer_clear(ringbuffer_t* buf)
{
    buf->start = 0;
    buf->end = 0;
}