/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "system/Streams.h"
#include <assert.h>
#include "system/handover/Handover.h"
#include "system/system/System.h"

static Handover _handover;

const char *entry_type_to_string[] = {
    "AVAILABLE",
    "RESERVED",
    "ACPI",
    "NVS",
    "BADRAM",
    "KERNEL",
};

Handover *handover()
{
    return &_handover;
}

void handover_assert(uint32_t magic)
{
    if (!(is_multiboot2(magic) ||
          is_stivale2(magic)))
    {
        system_panic("Wrong bootloader please use any multiboot/stival bootloader\n\tMagic number: 0x%08x!", magic);
    }
}