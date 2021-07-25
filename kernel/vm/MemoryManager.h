/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <base/Concepts.h>
#include <base/HashTable.h>
#include <base/NonnullOwnPtrVector.h>
#include <base/NonnullRefPtrVector.h>
#include <base/String.h>
#include <kernel/arch/x86/PageFault.h>
#include <kernel/arch/x86/TrapFrame.h>
#include <kernel/Forward.h>
#include <kernel/SpinLock.h>
#include <kernel/vm/AllocationStrategy.h>
#include <kernel/vm/PhysicalPage.h>
#include <kernel/vm/PhysicalRegion.h>
#include <kernel/vm/Region.h>
#include <kernel/vm/VMObject.h>

namespace Kernel {

constexpr bool page_round_up_would_wrap(FlatPtr x)
{
    return x > (explode_byte(0xFF) & ~0xFFF);
}

constexpr FlatPtr page_round_up(FlatPtr x)
{
    FlatPtr rounded = (((FlatPtr)(x)) + PAGE_SIZE - 1) & (~(PAGE_SIZE - 1));
    VERIFY(x == 0 || rounded != 0);
    return rounded;
}

constexpr FlatPtr page_round_down(FlatPtr x)
{
    return ((FlatPtr)(x)) & ~(PAGE_SIZE - 1);
}

inline FlatPtr low_physical_to_virtual(FlatPtr physical)
{
    return physical + kernel_base;
}

inline FlatPtr virtual_to_low_physical(FlatPtr virtual_)
{
    return virtual_ - kernel_base;
}

enum class UsedMemoryRangeType {
    LowMemory = 0,
    Prekernel,
    Kernel,
    BootModule,
    PhysicalPages,
};

static constexpr StringView UserMemoryRangeTypeNames[] {
    "Low memory",
    "Prekernel",
    "Kernel",
    "Boot module",
    "Physical Pages"
};

struct UsedMemoryRange {
    UsedMemoryRangeType type {};
    PhysicalAddress start;
    PhysicalAddress end;
};

struct ContiguousReservedMemoryRange {
    PhysicalAddress start;
    PhysicalSize length {};
};


}
