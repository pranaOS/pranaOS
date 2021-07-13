/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libio/Copy.h>
#include <libio/Sink.h>

namespace IO
{

static inline JResult skip(Reader &from, size_t n)
{
    Sink sink;
    return copy(from, sink, n);
}

}