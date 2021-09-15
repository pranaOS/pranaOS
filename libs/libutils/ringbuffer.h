/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <assert.h>
#include <string.h>
#include <libutils/std.h>

namespace Utils
{

template <typename T>
struct RingBuffer
{
private:
    size_t _head = 0;
    size_t _tail = 0;
    size_t _size = 0;
    size_t _used = 0;

    T *_buffer = nullptr;

public:
    RingBuffer(size_t size)
    {
        _size = size;
        _buffer = new T[size];
    }

    ~RingBuffer()
    {
        if (_buffer)
            delete[] _buffer;
    }

    bool empty() const
    {
        return _used == 0;
    }

    bool full() const
    {
        return _used == _size;
    }

    size_t used() const
    {
        return _used;
    }

    size_t write(const T *buffer, size_t size)
    {
        size_t written = 0;

        while (!full() && written < size)
        {
            put(buffer[written]);
            written++;
        }

        return written;
    }

};

}