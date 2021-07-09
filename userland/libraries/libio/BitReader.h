/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libio/Read.h>
#include <libutils/Assert.h>
#include <libutils/InlineRingBuffer.h>

namespace IO
{

struct BitReader
{
private:
    IO::Reader &_reader;
    InlineRingBuffer<uint8_t, 16> _buffer;
    size_t _head = 0;
    bool _end_of_file;



};
}