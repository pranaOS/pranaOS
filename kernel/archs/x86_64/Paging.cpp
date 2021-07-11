/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libutils/ResultOr.h>
#include "system/Streams.h"
#include "system/interrupts/Interupts.h"
#include "system/memory/Memory.h"
#include "system/system/System.h"
#include "archs/Arch.h"
#include "archs/x86_64/Paging.h"
#include "archs/x86_64/x86_64.h"

namespace Arch::x86_64
{

PML4 kpml4 ALIGNED(ARCH_PAGE_SIZE) = {};
PML3 kpml3 ALIGNED(ARCH_PAGE_SIZE) = {};
PML2 kpml2 ALIGNED(ARCH_PAGE_SIZE) = {};
PML1 kpml1[512] ALIGNED(ARCH_PAGE_SIZE) = {};

PML4 *kernel_pml4()
{
    return &kpml4;
}

void virtual_initialize()
{
    auto &pml4_entry = kpml4.entries[0];
    pml4_entry.user = 0;
    pml4_entry.writable = 1;
    pml4_entry.present = 1;
    pml4_entry.physical_address = (uint64_t)&kpml3 / ARCH_PAGE_SIZE;

    auto &pml3_entry = kpml3.entries[0];
    pml3_entry.user = 0;
    pml3_entry.writable = 1;
    pml3_entry.present = 1;
    pml3_entry.physical_address = (uint64_t)&kpml2 / ARCH_PAGE_SIZE;

    for (size_t i = 0; i < 512; i++)
    {
        auto &pml2_entry = kpml2.entries[i];
        pml2_entry.user = 0;
        pml2_entry.writable = 1;
        pml2_entry.present = 1;
        pml2_entry.physical_address = (uint64_t)&kpml1[i] / ARCH_PAGE_SIZE;
    }
}

}