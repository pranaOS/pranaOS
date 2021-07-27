/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <base/Types.h>
#include <kernel/arch/x86/CPUID.h>

namespace Kernel {

class MSR {
    uint32_t m_msr;

public:
    static bool have()
    {
        CPUID id(1);
        return (id.edx() & (1 << 5)) != 0;
    }

    MSR(const MSR&) = delete;
    MSR& operator=(const MSR&) = delete;

    MSR(uint32_t msr)
        : m_msr(msr)
    {
    }

    [[nodiscard]] u64 get()
    {
        u32 low, high;
        asm volatile("rdmsr"
                     : "=a"(low), "=d"(high)
                     : "c"(m_msr));
        return ((u64)high << 32) | low;
    }

    void set(u64 value)
    {
        u32 low = value & 0xffffffff;
        u32 high = value >> 32;
        asm volatile("wrmsr" ::"a"(low), "d"(high), "c"(m_msr));
    }
};

}