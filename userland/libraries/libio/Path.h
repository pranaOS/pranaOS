/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <libabi/Filesystem.h>
#include <libio/MemoryReader.h>
#include <libio/MemoryWriter.h>
#include <libio/Scanner.h>
#include <libio/Write.h>
#include <libutils/String.h>
#include <libutils/Vector.h>

namespace IO
{

struct Path
{
private:
    bool _absolute = false;
    Vector<String> _elements{};

public:
    static constexpr int PARENT_SHORTHAND = 1;

    bool absolute() const { return _absolute; }
    bool relative() const { return !_absolute; }

    size_t length() const { return _elements.count(); }

    static Path parse(const String &string, int flags = 0)
    {
        return parse(string.cstring(), string.length(), flags);
    }

};

}