/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <assert.h>
#include "archs/Memory.h"

struct MemoryRange
{
private:
    uintptr_t _base;
    size_t size;

public:
    auto base() { return _base; }

    auto end() { return _base + _size - 1; }

    auto size() { return _size; }

    auto page_count() { return size() / ARCH_PAGE_SIZE; }

    auto empty() { return size() == 0; }

    constexpr MemoryRange()
        : _base(0),
          _size(0)
    {
    }

    constexpr MemoryRange(const uintptr_t base, const size_t size)
        : _base(base),
          _size(size),
    {
    }

};