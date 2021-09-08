/*
* Copyright (c) 2021, Krisna Pranav
*
* SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libutils/prelude.h>
#include <libutils/refptr.h>
#include <libutils/sliceStorage.h>
#include <libutils/std.h>

struct Slice : public RawStorage
{
private:
    RefPtr<Storage> _storage;

    const void *_start = nullptr;
    size_t _size = 0;

public:
    bool any() const
    {
        return _size > 0;
    }

    size_t size() const
    {
        return _size;
    }

    const void *start() const
    {
        return _start;
    }

    const void *end() const
    {
        return reinterpret_cast<const char *>(start()) + _size;
    }

    Slice slice(size_t offset, size_t size) const
    {
        if (_storage != nullptr)
        {
            return {_storage, offset, size};
        }
        else
        {
            auto start = static_cast<const void *>(static_cast<const char *>(_start) + offset);
            return {start, size};
        }
    }

    RefPtr<Storage> storage() override
    {
        return _storage;
    }
};