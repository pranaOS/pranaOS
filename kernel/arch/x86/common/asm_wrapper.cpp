/**
 * @file asm_wrapper.cpp
 * @author Krisna Pranav
 * @brief ASM Wrapper
 * @version 6.0
 * @date 2024-09-18
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/types.h>
#include <kernel/arch/processor.h>
#include <kernel/arch/x86/asm_wrapper.h>
#include <kernel/sections.h>

namespace Kernel 
{

    #define XCR_XFEATURE_ENABLED_MASK 0

    UNMAP_AFTER_INIT u64 read_xcr0()
    {
        u32 eax, edx;
        asm volatile("xgetbv"
                    : "=a"(eax), "=d"(edx)
                    : "c"(XCR_XFEATURE_ENABLED_MASK));
        return eax + ((u64)edx << 32);
    }

    /**
     * @param value 
     * @return UNMAP_AFTER_INIT 
     */
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

} // namespace Kernel