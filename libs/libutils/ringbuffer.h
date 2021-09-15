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


};

}