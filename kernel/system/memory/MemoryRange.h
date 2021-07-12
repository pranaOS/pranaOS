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
};