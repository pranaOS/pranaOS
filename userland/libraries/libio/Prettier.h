/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libio/Write.h>
#include <libio/Writer.h>

namespace IO
{

struct Prettier :
    public IO::Writer
{
private:
    IO::Writer &_writer;
    int _depth = 0;
    int _flags;

public:
    static constexpr auto NONE = 0;
    static constexpr auto COLORS = 1 << 0;
    static constexpr auto INDENTS = 1 << 1;
    static constexpr auto USETAB = 1 << 2;

    Prettier(IO::Writer &writer, int flags = NONE)
        : _writer{writer},
          _flags{flags}
    {
    }

    ResultOr<size_t> write(const void *buffer, size_t size) override
    {
        return _writer.write(buffer, size);
    }

    JResult flush() override
    {
        return _writer.flush();
    }
    

};

}