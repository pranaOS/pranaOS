/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


// includes
#include "system/Streams.h"
#include "archs/x86/IOAPIC.h"

namespace Arch::x86
{

static volatile uint32_t *ioapic = nullptr;

void ioapic_found(uintptr_t address)
{
    ioapic = reinterpret_cast<uint32_t *>(address);
    Kernel::logln("IOAPIC found at {08x}", ioapic)
}
    
}
