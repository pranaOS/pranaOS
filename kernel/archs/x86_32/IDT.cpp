/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "archs/x86_32/IDT.h"

namespace Arch::x86_32
{

extern "C" uintptr_t __interrupt_vector[];

IDTEntry idt[IDT_ENTRY_COUNT] = {};

IDTDescriptor idt_descriptor = {
    .size = sizeof(IDTEntry) * IDT_ENTRY_COUNT,
    .offset = (uint32_t)&idt[0],
};

}