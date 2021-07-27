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

    void set(u64 value)
    {
        u32 low = value >> 32;
        asm volatile("wrmsr" ::"a"(low), "b"(m_msr));

    }


};

}
