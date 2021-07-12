/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "system/Streams.h"
#include "archs/x86/LAPIC.h"
#include "archs/x86/PIC.h"

namespace Arch::x86
{

constexpr int LAPIC_EOI = 0x00B0;

static volatile uint32_t *lapic = nullptr;

void lapic_found(uintptr_t address)
{
    lapic = reinterpret_cast<uint32_t *>(address);
    Kernel::logln("LAPIC found at {08x}", lapic);
}

uint32_t lapic_read(uint32_t reg)
{
    return *((volatile uint32_t *)(lapic + reg));
}

}