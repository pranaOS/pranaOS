/*
 * Copyright (c) 2021, nuke123-sudo
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libio/Reader.h>
#include <libmath/MinMax.h>
#include <libutils/Slice.h>

namespace IO
{

struct MemoryReader final :
    public Reader,
    public Seek
{

private:
    Slice _memory;
    size_t _position = 0;

public:
    Slice memory() { return _memory; }

    MemoryReader(const char *cstring)
        : _memory{cstring}
    {
    }

    MemoryReader(const void *ptr, size_t size)
        : _memory(ptr, size)
    {
    }

    MemoryReader(Slice memory)
        : _memory{memory}
    {
    }

    MemoryReader(RawStorage &raw)
        : _memory(raw.storage())
    {
    }

};

}