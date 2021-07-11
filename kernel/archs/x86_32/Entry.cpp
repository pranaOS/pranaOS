/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


// includes
#include <libsystem/core/Plugs.h>
#include "archs/x86/COM.h"
#include "archs/x86/FPU.h"
#include "archs/x86/PIC.h"
#include "archs/x86/PIT.h"
#include "archs/x86_32/GDT.h"
#include "archs/x86_32/IDT.h"
#include "system/graphics/Graphics.h"
#include "acpi/ACPI.h"
#include "smbios/SMBIOS.h"

namespace Arch::x86_32
{

extern "C" void arch_x86_32_main(void *info, uint32_t magic)
{
    com_initialize(COM1);
    com_initialize(COM2);
    com_initialize(COM3);
    com_initialize(COM4);
    
    auto handover = handover_initialize(info, magic);

    graphic_early_initialize(handover);

    if (handover->memory_usable < 127 * 1024)
    {
        system_panic("No enough memory (%uKio)!", handover->memory_usable / 1024);

    }

}

}