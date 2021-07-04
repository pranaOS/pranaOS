/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libutils/Prelude.h>
#include <libutils/RefPtr.h>
#include <libutils/SliceStorage.h>
#include <libutils/Std.h>

struct Slice :
    public RawStorage
{
private:
    RefPtr<Storage> _storage;

    const void *_start = nullptr;
    size_t size = 0;

public:
    bool any() const { return _size > 0; }

    size_t size() const { return _size; }

    const void *start() const 
    {
        return _start;
    }

    const void *end() const
    {
        return reinterpret_cast<const char *>(start()) + _size;
    }


}