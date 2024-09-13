/**
 * @file interruptdisabler.h
 * @author Krisna Pranav
 * @brief Interrupt Disabler
 * @version 6.0
 * @date 2024-09-13
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>
#include <mods/platform.h>
#include <kernel/arch/x86/io.h>
#include <kernel/arch/x86/asm_wrapper.h>

VALIDATE_IS_X86()

namespace Kernel 
{

    class InterruptDisabler 
    {
    public:
        /**
         * @brief Construct a new Interrupt Disabler object
         * 
         */
        InterruptDisabler()
        {
            m_flags = cpu_flags();
            cli();
        }

        /**
         * @brief Destroy the Interrupt Disabler object
         * 
         */
        ~InterruptDisabler()
        {
            if ((m_flags & 0x200) != 0)
                sti();
        }

    private:
        u32 m_flags;
    }; // class InterruptDisabler

    class NonMaskableInterruptDisabler 
    {
    public:
        /**
         * @brief Construct a new Non Maskable Interrupt Disabler object
         * 
         */
        NonMaskableInterruptDisabler()
        {
            IO::out8(0x70, IO::in8(0x70) | 0x80);
        }

        /**
         * @brief Destroy the Non Maskable Interrupt Disabler object
         * 
         */
        ~NonMaskableInterruptDisabler()
        {
            IO::out8(0x70, IO::in8(0x70) & 0x7F);
        }
    }; // class NonMaskableInterruptDisabler

} // namespace Kernel
