/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/Types.h>
#include <kernel/arch/x86/ASM_wrapper.h>
#include <kernel/arch/x86/Processor.h>
#include <kernel/Sections.h>

namespace Kernel {

#define XCR_XFEATURE_ENABLED_MASK 0

UNMAP_AFTER_INIT u64 read_xcr0()
{
    u32 eax, edx;
    asm volatile("xgetbv"
                 : "=a"(eax), "=d"(edx)
                 : "c"(XCR_XFEATURE_ENABLED_MASK));
    return eax + ((u64)edx << 32);
}

UNMAP_AFTER_INIT void write_xcr0(u64 value)
{
    u32 eax = value;
    u32 edx = value >> 32;
    asm volatile("xsetbv" ::"a"(eax), "d"(edx), "c"(XCR_XFEATURE_ENABLED_MASK));
}

void stac()
{
    if (!Processor::current().has_feature(CPUFeature::SMAP))
        return;
    asm volatile("stac" ::
                     : "cc");
}

void clac()
{
    if (!Processor::current().has_feature(CPUFeature::SMAP))
        return;
    asm volatile("clac" ::
                     : "cc");
}

}
