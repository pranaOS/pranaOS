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

    ResultOr<size_t> fill()
    {
        _used = TRY(_reader.read(_buffer, _size));
        _head = 0;

        return _used;
    }

    NONCOPYABLE(BufReader);
    NONMOVABLE(BufReader);

public:
    BufReader(Reader &reader, size_t size)
        : _reader{reader}, _size{size}
    {
        _buffer = new uint8_t[_size];
    }

    ~BufReader()
    {
        delete _buffer;
    }
    

};

}