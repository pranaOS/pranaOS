/**
 * @file asm_wrapper.h
 * @author Krisna Pranav
 * @brief ASM Wrapper
 * @version 6.0
 * @date 2024-10-24
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/arch/aarch64/registers.h>

namespace Kernel::Aarch64::Asm 
{

    /**
     * @brief Set the ttbr1 el1 object
     * 
     * @param ttbr1_el1 
     */
    inline void set_ttbr1_el1(FlatPtr ttbr1_el1)
    {
        asm("msr ttbr1_el1, %[value]" ::[value] "r"(ttbr1_el1));
    }

    /**
     * @brief Set the ttbr0 el1 object
     * 
     * @param ttbr0_el1 
     */
    inline void set_ttbr0_el1(FlatPtr ttbr0_el1)
    {
        asm("msr ttbr0_el1, %[value]" ::[value] "r"(ttbr0_el1));
    }

    inline void flush()
    {
        asm("dsb ish");
        asm("isb");
    }

    [[noreturn]] inline void halt()
    {
        for (;;) {
            asm volatile("wfi");
        }
    }

    enum class ExceptionLevel : u8 
    {
        EL0 = 0,
        EL1 = 1,
        EL2 = 2,
        EL3 = 3,
    }; // enum class ExceptionLevel : u8 

    inline ExceptionLevel get_current_exception_level()
    {
        u64 current_exception_level;

        asm("mrs  %[value], CurrentEL"
            : [value] "=r"(current_exception_level));

        current_exception_level = (current_exception_level >> 2) & 0x3;
        return static_cast<ExceptionLevel>(current_exception_level);
    }

} // namespace Kernel::Aarch64::Asm 