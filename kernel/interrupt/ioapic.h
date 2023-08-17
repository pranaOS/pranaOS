/**
 * @file ioapic.h
 * @author Krisna Pranav
 * @brief ioapic
 * @version 1.0
 * @date 2023-08-17
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/interrupt/irqcontroller.h>
#include <kernel/vm/typedmapping.h>

namespace Kernel
{
    struct [[gnu::packed]] ioapic_mmio_regs
    {
        volatile u32_select;
        u32 reserved[3];
        volatile u32 window;
    }; // struct ioapic_mmio_regs

    class PCIInterruptOverrideMetadata
    {  
    public:
        /**
         * @param bus_id 
         * @param polarity 
         * @param trigger_mode 
         */
        PCIInterruptOverrideMetadata(u8 bus_id, u8 polarity, u8 trigger_mode);

        /**
         * @return u8 
         */
        u8 bus() const
        {
            return m_bus_id;
        }

    private:
        const u8 m_bus_id;
        const u8 m_polarity;
        const u8 m_trigger_mode;
        const u8 m_pci_interrupt_pin;
        const u8 m_pci_device_number;
        const u16 m_ioapic_id;
        const u16 ioapic_interrupt_pin;
    }; // class PCIInterruptOverrideMetadata

    class IOAPIC final : public IRQController
    {
    private:
        PhysicalAddress m_address;
        mutable TypedMapping<ioapic_mmio_regs> m_regs;
        u32 m_gsi_base;
        u8 m_id;
        u8 m_version;
        size_t m_redirection_entries_count;
    }; //

} // namespace Kernel