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

public:
    inline BitReader(IO::Reader &reader) : _reader(reader) {}

    inline JResult hint(size_t num_bits)
    {
        size_t num_bytes = ALIGN_UP(_head + num_bits, 8) / 8;

        while (_buffer.used() < num_bytes)
        {
            uint8_t byte;

            if (TRY(_reader.read(&byte, sizeof(byte))) == 0)
            {
                _end_of_file = true;
                return SUCCESS;
            }
            else
            {
                _buffer.put(byte);
            }
        }

        return SUCCESS;
    }
    

};
}