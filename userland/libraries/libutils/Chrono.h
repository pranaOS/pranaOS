/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libabi/Syscalls.h>
#include <libio/Streams.h>

namespace Utils
{

struct Chrono
{
private:
    const char *_name;
    Tick _start;

    NONMOVABLE(Chrono);
    NONCOPYABLE(Chrono);

}
}