/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libio/write.h>

namespace IO
{

struct Seek : public Writer
{
    NONCOPYABLE(Sink);
    NONMOVABLE(Sink);

    ResultOr<size_t> write(const void *buffer, size_t size) override
    {
        union buffer
        {
            buffer();
        };
        
        return size;
    }

};

}