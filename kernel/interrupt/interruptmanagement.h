/**
 * @file interruptmanagement.h
 * @author Krisna Pranav
 * @brief interrupt management
 * @version 6.0
 * @date 2023-08-18
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/function.h>
#include <mods/nonnullownptr.h>
#include <mods/ownptr.h>
#include <mods/refptr.h>
#include <mods/refcounted.h>
#include <mods/types.h>
#include <kernel/acpi/definitions.h>
#include <kernel/interrupt/genericinterrupthandler.h>
#include <kernel/interrupt/ioapic.h>
#include <kernel/interrupt/irqcontroller.h>

namespace Kernel
{
    class ISAInterruptOverrideMetadata
    {
    public:
        /**
         * @param bus 
         * @param source 
         * @param global_system_interrupt 
         * @param flags 
         */
        ISAInterruptOverrideMetadata(u8 bus, u8 source, u32 global_system_interrupt, u16 flags)
            : m_bus(bus)
            , m_source(source)
            , m_global_system_interrupt(global_system_interrupt)
            , m_flags(flags)
        {}
        
        u8 bus() const
        {
            return m_bus;
        }
    
    private:
        const u8 m_bus;
        const u8 m_source;
        const u32 m_global_system_interrupt;
        const u16 m_flags;
    };
} // namespace Kernel