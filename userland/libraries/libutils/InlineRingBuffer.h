/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libutils/Std.h>

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