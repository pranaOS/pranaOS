/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <string.h>
#include <libio/Reader.h>
#include <libmath/MinMax.h>

namespace IO
{

struct BufReader : public Reader
{
private:
    Reader &_reader;

    uint8_t *_buffer;
    size_t _used = 0;
    size_t _head = 0;
    size_t _size = 0;
    

};

}