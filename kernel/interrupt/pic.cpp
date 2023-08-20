/**
 * @file pic.cpp
 * @author Krisna Pranav
 * @brief PIC
 * @version 6.0
 * @date 2023-08-20
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/assertions.h>
#include <mods/types.h>
#include <kernel/arch/i386/cpu.h>
#include <kernel/interrupt/genericinterrupthandler.h>
#include <kernel/interrupt/pic.h>
#include <kernel/io.h>

namespace Kernel
{
    /**
     * @param reg 
     * @return true 
     * @return false 
     */
    bool inline static is_all_masked(u16 reg)
    {
        return reg == 0xFFFF;
    }

    /**
     * @return true 
     * @return false 
     */
    bool PIC::is_enabled() const
    {
        return !is_all_masked(m_cached_irq_mask) && !is_hard_disabled();
    }

    /**
     * @param handler 
     */
    void PIC::disable(const GenericInterruptHandler& handler)
    {
        InterruptDisabler disabler;

        ASSERT(!is_hard_disabled());
        ASSERT(handler.interrupt_number() >= gsi_base());

        u8 irq = handler.interrupt_number();

        if (m_cached_irq_mask & (1 << irq))
            return;
        
        u8 imr;
    }

    PIC::PIC()
    {
        initialize();
    }
}