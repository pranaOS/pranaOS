/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libabi/Handle.h>
#include <libutils/ResultOr.h>

namespace IO
{

enum struct Whence : uint8_t
{
    START = J_WHENCE_START,
    CURRENT = J_WHENCE_CURRENT,
    END = J_WHENCE_END,
};

struct SeekFrom
{
    Whence whence;
    ssize64_t position;

    static SeekFrom start(ssize64_t position = 0)
    {
        return {Whence::START, position};
    }
    
    static SeekFrom current(ssize64_t position = 0)
    {
        return {Whence::CURRENT, position};
    }

    static SeekFrom end(ssize64_t position = 0)
    {
        return {Whence::END, position};
    }
};

struct Seek
{
    virtual ~Seek() {}

    virtual ResultOr<size_t> seek(SeekFrom from) = 0;

};

}