/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "archs/x86_32/GDT.h"

namespace Arch::x86_32
{

static TSS tss = {
    .prev_tss = 0,
    .esp0 = 0,
    .ss0 = 0x10,
    .esp1 = 0,
    .ss1 = 0,
    .esp2 = 0,
    .ss2 = 0,
    .cr3 = 0,
    .eip = 0,
    .eflags = 0x0202,
    .eax = 0,
    .ecx = 0,
    .edx = 0,
    .ebx = 0,
    .esp = 0,
    .ebp = 0,
    .esi = 0,
    .edi = 0,
    .es = 0,
    .cs = 0,
    .ss = 0,
    .ds = 0,
    .fs = 0,
    .gs = 0,
    .ldt = 0,
    .trap = 0,
    .iomap_base = 0,
};

static GDTEntry gdt[GDT_ENTRY_COUNT];

static GDTDescriptor gdt_descriptor = {
    .size = sizeof(GDTEntry) * GDT_ENTRY_COUNT,
    .offset = (uint32_t)&gdt[0],
};


}