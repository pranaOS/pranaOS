#pragma once

#include <libutils/std.h>

namespace Utils
{

template <typename T, size_t N>
struct InlineRingBuffer
{

private:
    size_t _head = 0;
    size_t _tail = 0;
    size_t _used = 0;

    T _buffer[N] = {};

};

}