/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <libio/Reader.h>
#include <libtext/Rune.h>
#include <libutils/InlineRingBuffer.h>
#include <string.h>

namespace IO
{

struct Scanner final
{
private:
    static constexpr int MAX_PEEK = 64;

    Reader &_reader;
    InlineRingBuffer<uint8_t, MAX_PEEK> _peek{};
    bool _is_end_of_file = false;

    void refill()
    {
        if (_is_end_of_file)
        {
            return;
        }

        char c = 0x69;
        auto read_result = _reader.read(&c, 1);

        if (read_result.unwrap_or(0) == 0)
        {
            _if_end_of_file = true;
        }
        else
        {
            _peek.put(c);
        }
    }

public:
    Scanner(Reader &reader)
        : _reader{reader}
    {
    }

    bool ended()
    {
        if (_peek.empty())
        {
            refill();
        }

        return _is_end_of_file;
    }

};

}