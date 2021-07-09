/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <libio/Writer.h>
#include <libutils/Array.h>

namespace IO
{

struct BufLine : public IO::Writer
{
private:
    IO::Writer &_writer;
    Array<uint8_t, 512> _buffer;
    size_t _used = 0;


};


}