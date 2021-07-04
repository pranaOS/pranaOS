/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "system/Streams.h"
#include <libsystem/BuildInfo.h>
#include "archs/Arch.h"
#include "system/scheduling/Scheduler.h"
#include "system/system/System.h"

void system_hang()
{
    while (true)
    {
        Arch::halt();
    }
}

void system_stop()
{
    Arch::disable_interrupts();
    Kernel::logln("System stopped due to issues!!!");

    while (1)
    {
        Arch::disable_interrupts();
        Arch::halt();
    }
}

struct uint32_t _system_tick;

void system_tick()
{
    if (_system_tick + 1 < _system_tick)
    {
        system_panic("System tick overflowed!!!");
    }

    _system_tick++;
}
