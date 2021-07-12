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

void reboot_triple_fault()
{
    Kernel::logln("Trying to reboot by doing a triple fault...");
    cli();
    Arch::address_space_switch(0x0);
    *(uint32_t *)0x0 = 0xDEADDEAD;
}

void shutdown_virtual_machines()
{
    Kernel::logln("Maybe your are running a VM, trying to shutdown using io ports...");

    out16(0xB004, 0x2000);

    out16(0x604, 0x2000);

    out16(0x4004, 0x3400);
}

void shutdown_acpi()
{
    Kernel::logln("Trying to shutdown using acpi...");
}

}