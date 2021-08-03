/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// incldues
#include <base/Optional.h>
#include <kernel/arch/x86/InterruptDisabler.h>
#include <kernel/Debug.h>
#include <kernel/interrupts/APIC.h>
#include <kernel/interrupts/IOAPIC.h>
#include <kernel/interrupts/InterruptManagement.h>
#include <kernel/Sections.h>

#define IOAPIC_REDIRECTION_ENTRY_OFFSET 0x10
namespace Kernel {
enum DeliveryMode {
    Normal = 0,
    LowPriority = 1,
    SMI = 2,
    NMI = 3,
    INIT = 4,
    External = 7
};

UNMAP_AFTER_INIT IOAPIC::IOAPIC(PhysicalAddress address, u32 gsi_base)
    : m_address(address)
    , m_regs(map_typed_writable<ioapic_mmio_regs>(m_address))
    , m_gsi_base(gsi_base)
    , m_id((read_register(0x0) >> 24) & 0xFF)
    , m_version(read_register(0x1) & 0xFF)
    , m_redirection_entries_count((read_register(0x1) >> 16) + 1)
{
    InterruptDisabler disabler;
    dmesgln("IOAPIC ID: {:#x}", m_id);
    dmesgln("IOAPIC Version: {:#x}, redirection entries: {}", m_version, m_redirection_entries_count);
    dmesgln("IOAPIC Arbitration ID {:#x}", read_register(0x2));
    mask_all_redirection_entries();
}

UNMAP_AFTER_INIT void IOAPIC::initialize()
{
}

void IOAPIC::map_interrupt_redirection(u8 interrupt_vector)
{
    InterruptDisabler disabler;
    for (auto redirection_override : InterruptManagement::the().isa_overrides()) {
        if (redirection_override.source() != interrupt_vector)
            continue;
        bool active_low = false;
        switch ((redirection_override.flags() & 0b11)) {
        case 0:
            active_low = false;
            break;
        case 1:
            active_low = false;
            break;
        case 2:
            VERIFY_NOT_REACHED(); 
        case 3:
            active_low = true;
            break;
        }

        bool trigger_level_mode = false;

        switch (((redirection_override.flags() >> 2) & 0b11)) {
        case 0:
            trigger_level_mode = false;
            break;
        case 1:
            trigger_level_mode = false;
            break;
        case 2:
            VERIFY_NOT_REACHED();
        case 3:
            trigger_level_mode = true;
            break;
        }
        configure_redirection_entry(redirection_override.gsi() - gsi_base(), InterruptManagement::acquire_mapped_interrupt_number(redirection_override.source()) + IRQ_VECTOR_BASE, DeliveryMode::Normal, false, active_low, trigger_level_mode, true, 0);
        return;
    }
    isa_identity_map(interrupt_vector);
}

void IOAPIC::isa_identity_map(int index)
{
    InterruptDisabler disabler;
    configure_redirection_entry(index, InterruptManagement::acquire_mapped_interrupt_number(index) + IRQ_VECTOR_BASE, DeliveryMode::Normal, false, false, false, true, 0);
}

void IOAPIC::map_pci_interrupts()
{
    InterruptDisabler disabler;
    configure_redirection_entry(11, 11 + IRQ_VECTOR_BASE, DeliveryMode::Normal, false, false, true, true, 0);
}

bool IOAPIC::is_enabled() const
{
    return !is_hard_disabled();
}

void IOAPIC::spurious_eoi(const GenericInterruptHandler& handler) const
{
    InterruptDisabler disabler;
    VERIFY(handler.type() == HandlerType::SpuriousInterruptHandler);
    VERIFY(handler.interrupt_number() == APIC::spurious_interrupt_vector());
    dbgln("IOAPIC: Spurious interrupt");
}

void IOAPIC::map_isa_interrupts()
{
    InterruptDisabler disabler;
    for (auto redirection_override : InterruptManagement::the().isa_overrides()) {
        if ((redirection_override.gsi() < gsi_base()) || (redirection_override.gsi() >= (gsi_base() + m_redirection_entries_count)))
            continue;
        bool active_low = false;

        switch ((redirection_override.flags() & 0b11)) {
        case 0:
            active_low = false;
            break;
        case 1:
            active_low = false;
            break;
        case 2:
            VERIFY_NOT_REACHED();
        case 3:
            active_low = true;
            break;
        }

        bool trigger_level_mode = false;

        switch (((redirection_override.flags() >> 2) & 0b11)) {
        case 0:
            trigger_level_mode = false;
            break;
        case 1:
            trigger_level_mode = false;
            break;
        case 2:
            VERIFY_NOT_REACHED();
        case 3:
            trigger_level_mode = true;
            break;
        }
        configure_redirection_entry(redirection_override.gsi() - gsi_base(), InterruptManagement::acquire_mapped_interrupt_number(redirection_override.source()) + IRQ_VECTOR_BASE, 0, false, active_low, trigger_level_mode, true, 0);
    }
}

void IOAPIC::reset_all_redirection_entries() const
{
    InterruptDisabler disabler;
    for (size_t index = 0; index < m_redirection_entries_count; index++)
        reset_redirection_entry(index);
}

void IOAPIC::hard_disable()
{
    InterruptDisabler disabler;
    reset_all_redirection_entries();
    IRQController::hard_disable();
}

void IOAPIC::reset_redirection_entry(int index) const
{
    InterruptDisabler disabler;
    configure_redirection_entry(index, 0, 0, false, false, false, true, 0);
}

void IOAPIC::configure_redirection_entry(int index, u8 interrupt_vector, u8 delivery_mode, bool logical_destination, bool active_low, bool trigger_level_mode, bool masked, u8 destination) const
{
    InterruptDisabler disabler;
    VERIFY((u32)index < m_redirection_entries_count);
    u32 redirection_entry1 = interrupt_vector | (delivery_mode & 0b111) << 8 | logical_destination << 11 | active_low << 13 | trigger_level_mode << 15 | masked << 16;
    u32 redirection_entry2 = destination << 24;
    write_register((index << 1) + IOAPIC_REDIRECTION_ENTRY_OFFSET, redirection_entry1);

    if constexpr (IOAPIC_DEBUG)
        dbgln("IOAPIC Value: {:#x}", read_register((index << 1) + IOAPIC_REDIRECTION_ENTRY_OFFSET));

    write_register((index << 1) + IOAPIC_REDIRECTION_ENTRY_OFFSET + 1, redirection_entry2);

    if constexpr (IOAPIC_DEBUG)
        dbgln("IOAPIC Value: {:#x}", read_register((index << 1) + 0x11));
}

void IOAPIC::mask_all_redirection_entries() const
{
    InterruptDisabler disabler;
    for (size_t index = 0; index < m_redirection_entries_count; index++)
        mask_redirection_entry(index);
}


}