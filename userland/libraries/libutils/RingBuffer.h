/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <assert.h>
#include <string.h>
#include <libutils/Std.h>

namespace Utils
{

template <typename T>
struct RingBuffer
{
private:
    size_t _head = 0;
    size_t _tail = 0;
    size_t _size = 0;
    size_t _used = 0

    T *_buffer = nullptr;

public:
    RingBuffer(size_t size)
    {
        _size = size;
        _buffer = new T[size];
    }

    RingBuffer(const RingBuffer &other) : _head(other._head),
                                          _tail(other._tail),
                                          _size(other._size),
                                          _used(other._used)
    {
        _buffer = new T[other._size];
        memcpy(_buffer, other._buffer, other._size);
    }

    RingBuffer(RingBuffer &&other)
        : _head(other._head),
          _tail(other._tail),
          _size(other._size),
          _used(other._used),
          _buffer(other._buffer)
    {
        other._head = 0;
        other._tail = 0;
        other._size = 0;
        other._used = 0;
        other._buffer = nullptr;
    }

    RingBuffer &operator=(const RingBuffer &other)
    {
        return *this = RingBuffer(other);
    }

    RingBuffer &operator=(RingBuffer &&other)
    {
        std::swap(_head, other._head);
        std::swap(_tail, other._tail);
        std::swap(_size, other._size);
        std::swap(_used, other._used);
        std::swap(_buffer, other._buffer);

        return *this;
    }
}
}