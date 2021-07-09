/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libio/Write.h>
#include <libutils/Prelude.h>

namespace IO
{

struct BitWriter
{

private:
    uint_fast32_t _bit_buffer;
    uint8_t _bit_count;
    Writer &_writer;

public:
    BitWriter(Writer &writer) : _writer(writer)
    {
    }

    ~BitWriter()
    {
        flush();
    }

};

}