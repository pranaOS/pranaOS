/**
 * @file msr.h
 * @author Krisna Pranav
 * @brief MSR
 * @version 6.0
 * @date 2024-09-15
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>
#include <mods/platform.h>
#include <kernel/arch/x86/cpuid.h>


VALIDATE_IS_X86()

namespace Kernel 
{
    class MSR 
    {
        uint32_t m_msr;
    public:

        /**
         * @return true 
         * @return false 
         */
        static bool have()
        {
            CPUID id(1);
            return (id.edx() & (1 << 5)) != 0;
        }

        /**
         * @brief Construct a new MSR object
         * 
         */
        MSR(const MSR&) = delete;

        /**
         * @return MSR& 
         */
        MSR& operator=(const MSR&) = delete;

        /**
         * @brief Construct a new MSR object
         * 
         * @param msr 
         */
        MSR(uint32_t msr)
            : m_msr(msr)
        {
        }

        /**
         * @return u64 
         */
        [[nodiscard]] u64 get()
        {
            u32 low, high;
            asm volatile("rdmsr"
                        : "=a"(low), "=d"(high)
                        : "c"(m_msr));
            return ((u64)high << 32) | low;
        }

        /**
         * @param value 
         */
        void set(u64 value)
        {
            u32 low = value & 0xffffffff;
            u32 high = value >> 32;
            asm volatile("wrmsr" ::"a"(low), "d"(high), "c"(m_msr));
        }
    }; // class MSR
} // namespace Kernel