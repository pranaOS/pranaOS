/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
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

public:
    Chrono(const char *name)
    {
        _name = name;
        hj_system_tick(&_start);
    }

    ~Chrono()
    {
        Tick end;
        hj_system_tick(&end);

        IO::logln("Chrono '{}' ended at {}", _name, end - _start);
    }
};

} 