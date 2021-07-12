/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "system/Streams.h"
#include "archs/Arch.h"
#include "archs/x86/IOPort.h"
#include "archs/x86/Power.h"
#include "archs/x86/x86.h"

namespace Arch::x86
{

void reboot_8042()
{
    Kernel::logln("Trying to reboot using the 8042...");

    uint8_t good = 0x02;

    while (good & 0x02)
    {
        good = in8(0x64);
    }

    out8(0x64, 0xFE);
    Arch::halt();
}

}