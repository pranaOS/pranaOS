/**
 * @file interruptmanagement.h
 * @author Krisna Pranav
 * @brief interrupt management
 * @version 6.0
 * @date 2023-08-18
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
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

namespace Kernel {

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
        {
        }

        /**
         * @return u8 
         */
        u8 bus() const 
        { 
            return m_bus; 
        }

        u8 source() const 
        { 
            return m_source; 
        }

        /**
         * @return u32 
         */
        u32 gsi() const 
        { 
            return m_global_system_interrupt; 
        }

        /**
         * @return u16 
         */
        u16 flags() const 
        { 
            return m_flags;
        }

    private:
        const u8 m_bus;
        const u8 m_source;
        const u32 m_global_system_interrupt;
        const u16 m_flags;
    }; // class ISAInterruptOverrideMetadata

    class InterruptManagement 
    {
    public:
        /**
         * @return InterruptManagement& 
         */
        static InterruptManagement& the();

        /// @breif: initialize + initialized
        static void initialize();
        static bool initialized();

        /**
         * @param original_irq 
         * @return u8 
         */
        static u8 acquire_mapped_interrupt_number(u8 original_irq);

        /**
         * @param mapped_interrupt_vector 
         * @return u8 
         */
        static u8 acquire_irq_number(u8 mapped_interrupt_vector);

        /// @brief: switch to pic (or) ioapic mode
        virtual void switch_to_pic_mode();
        virtual void switch_to_ioapic_mode();

        /**
         * @return true 
         * @return false 
         */
        bool smp_enabled() const 
        { 
            return m_smp_enabled; 
        }

        /**
         * @param interrupt_vector 
         * @return RefPtr<IRQController> 
         */
        RefPtr<IRQController> get_responsible_irq_controller(u8 interrupt_vector);

        /**
         * @return const Vector<ISAInterruptOverrideMetadata>& 
         */
        const Vector<ISAInterruptOverrideMetadata>& isa_overrides() const { return m_isa_interrupt_overrides; }

        /**
         * @param original_irq 
         * @return u8 
         */
        u8 get_mapped_interrupt_vector(u8 original_irq);
        
        /**
         * @param mapped_interrupt_vector 
         * @return u8 
         */
        u8 get_irq_vector(u8 mapped_interrupt_vector);

        /// @brief: enumerate interrupt handlers
        void enumerate_interrupt_handlers(Function<void(GenericInterruptHandler&)>);

        /**
         * @param index 
         * @return IRQController& 
         */
        IRQController& get_interrupt_controller(int index);

    private:

        InterruptManagement();
        PhysicalAddress search_for_madt();

        void locate_apic_data();

        bool m_smp_enabled { false };

        Vector<RefPtr<IRQController>> m_interrupt_controllers;
        Vector<ISAInterruptOverrideMetadata> m_isa_interrupt_overrides;
        Vector<PCIInterruptOverrideMetadata> m_pci_interrupt_overrides;

        PhysicalAddress m_madt;
    }; // class InterruptManagement

} // namespace Kernel
