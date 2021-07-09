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

    ResultOr<size_t> buffered()
    {
        return _used;
    }

    ResultOr<size_t> read(void *buffer, size_t size) override
    {
        size_t data_left = size;
        uint8_t *data_to_read = (uint8_t *)buffer;

        while (data_left != 0)
        {

            if (_head == _used)
            {
                if (TRY(fill()) == 0)
                {
                    return size - data_left;
                }
            }

            size_t used_space = _used - _head;
            size_t data_added = MIN(used_space, data_left);

            memcpy(
                data_to_read,
                ((uint8_t *)_buffer) + _head,
                data_added);

            data_left -= data_added;
            _head += data_added;
            data_to_read += data_added;
        }

        return size - data_left;
    }
};

}