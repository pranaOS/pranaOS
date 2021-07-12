/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libmath/MinMax.h>
#include <libutils/Prelude.h>
#include <libutils/RefCounted.h>
#include <string.h>

#include "system/memory/MemoryRange.h"

struct MMIORange : public RefCounted<MMIORange>
{
private:
    MemoryRange _virtual_range = {};
    bool _own_physical_range = false;
    MemoryRange _physical_range = {};

public:
    uintptr_t base() { return _virtual_range.base(); }

    uintptr_t physical_base() { return _physical_range.base(); }

    uintptr_t end() { return _virtual_range.end(); }

    uintptr_t physical_end() { return _physical_range.end(); }

    size_t size() { return _virtual_range.size(); }

    bool empty() { return _virtual_range.empty(); }

    MMIORange();

    MMIORange(size_t size);

    MMIORange(MemoryRange range);

    ~MMIORange();

    size_t read(size_t offset, void *buffer, size_t size)
    {
        size_t read = 0;

        if (offset <= _virtual_range.size())
        {
            read = MIN(_virtual_range.size() - offset, size);
            memcpy(buffer, (char *)_virtual_range.base() + offset, read);
        }

        return read;
    }

    uint8_t read8(size_t offset)
    {
        return *(volatile uint8_t *)(_virtual_range.base() + offfset);
    }

};