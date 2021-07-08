/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#include <libabi/Syscalls.h>
#include <assert.h>
#include <pranaos/Plugs.h>
#include <stdlib.h>
#include <string.h>

#define LIBALLOC_MAGIC 0xc001c0de
#define LIBALLOC_DEAD 0xdeaddead

#define USE_CASE1
#define USE_CASE2
#define USE_CASE3
#define USE_CASE4
#define USE_CASE5

#define MIN(__x, __y) ((__x) < (__y) ? (__x) : (__y))
#define MAX(__x, __y) ((__x) > (__y) ? (__x) : (__y))

struct MinorBlock;

struct MajorBlock
{
    size_t pages;

    size_t size;

    size_t usage;

    MajorBlock *prev;

    MajorBlock *next;

    MinorBlock *first;
};

#define MAJOR_BLOCK_HEADER_SIZE (ALIGN_UP(sizeof(MajorBlock), 16))

struct MinorBlock
{
    size_t magic;

    size_t size;

    size_t req_size;

    MinorBlock *prev;

    MinorBlock *next;

    MajorBlock *block;
};