/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <libkern/types.h>

struct bitmap {
    uint8_t* data;
    uint32_t len;
};
typedef struct bitmap bitmap_t;

bitmap_t bitmap_wrap(uint8_t* data, uint32_t len);
bitmap_t bitmap_allocate(uint32_t len);
int bitmap_find_space(bitmap_t bitmap, int req);
int bitmap_find_space_aligned(bitmap_t bitmap, int req, int alignment);
int bitmap_set(bitmap_t bitmap, int where);
int bitmap_unset(bitmap_t bitmap, int where);
int bitmap_set_range(bitmap_t bitmap, int start, int len);
int bitmap_unset_range(bitmap_t bitmap, int start, int len);