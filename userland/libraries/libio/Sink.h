/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libio/Write.h>

namespace IO
{

struct Sink :
    public Writer
{
    ResultOr<size_t> write(const void *buffer, size_t size) override
    {
        UNUSED(buffer);
        return size
    }
};

}