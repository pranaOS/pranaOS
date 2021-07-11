/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "archs/x86_64/IDT.h"

namespace Arch::x86_64
{

extern "C" uintptr_t __interrupt_vector[];

IDT64Entry idt[IDT_ENTRY_COUNT] = {};

IDT64Descriptor idt_descriptor = {
    .size = sizeof(IDT64Entry) * IDT_ENTRY_COUNT,
    .offset = (uint64_t)&idt[0],
};

}