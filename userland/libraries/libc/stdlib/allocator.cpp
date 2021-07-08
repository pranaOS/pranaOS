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

#define MINOR_BLOCK_HEADER_SIZE (ALIGN_UP(sizeof(MinorBlock), 16))

static MajorBlock *_heap_root = nullptr;

static MajorBlock *_best_bet = nullptr;

static constexpr size_t _page_size = 4096;

static constexpr size_t _page_count = 16;

static MajorBlock *heap_major_block_create(size_t size)
{
    // This is how much space is required.
    size_t st = size + MAJOR_BLOCK_HEADER_SIZE;
    st += MINOR_BLOCK_HEADER_SIZE;

    // Perfect amount of space?
    if ((st % _page_size) == 0)
    {
        st = st / (_page_size);
    }
    else
    {
        st = st / (_page_size) + 1;
    }

    // Make sure it's >= the minimum size.
    st = MAX(st, _page_count);

    MajorBlock *maj = (MajorBlock *)__plug_memory_alloc(st * _page_size);

    if (maj == nullptr)
    {
        return nullptr;
    }

    maj->prev = nullptr;
    maj->next = nullptr;
    maj->pages = st;
    maj->size = st * _page_size;
    maj->usage = MAJOR_BLOCK_HEADER_SIZE;
    maj->first = nullptr;

    return maj;
}
