/*
 * Copyright (c) 2021, nuke123-sudo
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <libio/Writer.h>
#include <libmath/MinMax.h>
#include <libutils/SliceStorage.h>
#include <libutils/StringStorage.h>

namespace IO
{

struct MemoryWriter :
    public Writer,
    public Seek 
{
private:
    size_t _used = 0;
    size_t _size = 0;
    size_t _position = 0;
    uint8_t *_buffer = nullptr;

public:
    using Writer::flush;

    MemoryWriter() : MemoryWriter(16)
    {
    }

    MemoryWriter(size_t preallocated)
    {
        preallocated = MAX(preallocated, 16);

        _buffer = new uint8_t[preallocated];
        _buffer[0] = '\0';
        _size = preallocated;
        _used = 0;
        _position = 0;
    }

    ~MemoryWriter()
    {
        if (_buffer)
        {
            delete[] _buffer;
        }
    }

};

}