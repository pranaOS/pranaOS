/*
* Copyright (c) 2021, Krisna Pranav
*
* SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libabi/syscalls.h>
#include <libio/streams.h>

namespace Utils
{

struct Chrono
{
private:
    const char *_name;
    Tick _start;

    NONMOVABLE(Chrono);
    NONCOPYABLE(Chrono);
};

}
