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
}