/**
 * @file ioapic.cpp
 * @author Krisna Pranav
 * @brief ioapic
 * @version 6.0
 * @date 2023-08-20
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/optional.h>
#include <mods/string_view.h>
#include <kernel/acpi/multiprocessorparser.h>
#include <kernel/arch/i386/cpu.h>
#include <kernel/interrupt/apic.h>
#include <kernel/interrupt/ioapic.h>
#include <kernel/interrupt/interruptmanagement.h>
#include <kernel/vm/memorymanager.h>

#define IOAPIC_REDIRECTION_ENTRY_OFFSET 0x10

namespace Kernel 
{
    enum DeliveryMode 
    {
        Normal = 0,
        LowPriority = 1,
        SMI = 2,
        NMI = 3,
        INIT = 4,
        External = 7
    }; // enum

    /**
     * @param address 
     * @param gsi_base 
     */
    IOAPIC::IOAPIC(PhysicalAddress address, u32 gsi_base)
        : m_address(address)
        , m_regs(map_typed_writable<ioapic_mmio_regs>(m_address))
        , m_gsi_base(gsi_base)
        , m_id((read_register(0x0) >> 24) & 0xFF)
        , m_version(read_register(0x1) & 0xFF)
        , m_redirection_entries_count((read_register(0x1) >> 16) + 1)
    {
        InterruptDisabler disabler;

        klog() << "IOAPIC *ID'S*: 0x" << String::format("%x", m_id);
        klog() << "IOAPIC Version: 0x" << String::format("%x", m_version) << ", Redirection Entries count - " << m_redirection_entries_count;
        klog() << "IOAPIC Arbitration *ID* 0x" << String::format("%x", read_register(0x2));

        mask_all_redirection_entries();
    }

    /// @brief: initialize
    void IOAPIC::initialize()
    {
    }
    
    /**
     * @param interrupt_vector 
     */
    void IOAPIC::map_interrupt_redirection(u8 interrupt_vector)
    {
        InterruptDisabler disabler;

        for (auto redirection_override : InterruptManagement::the().isa_overrides()) {

            if (redirection_override.source() != interrupt_vector)
                continue;

            bool active_low;

            switch ((redirection_override.flags() & 0b11)) {
            case 0:
                active_low = false;
                break;
            case 1:
                active_low = false;
                break;
            case 2:
                ASSERT_NOT_REACHED();
            case 3:
                active_low = true;
                break;
            }

            bool trigger_level_mode;

            switch (((redirection_override.flags() >> 2) & 0b11)) {
            case 0:
                trigger_level_mode = false;
                break;
            case 1:
                trigger_level_mode = false;
                break;
            case 2:
                ASSERT_NOT_REACHED(); 
            case 3:
                trigger_level_mode = true;
                break;
            }

            configure_redirection_entry(redirection_override.gsi() - gsi_base(), InterruptManagement::acquire_mapped_interrupt_number(redirection_override.source()) + IRQ_VECTOR_BASE, DeliveryMode::Normal, false, active_low, trigger_level_mode, true, 0);

            return;
        }

        isa_identity_map(interrupt_vector);
    }

    /**
     * @param index 
     */
    void IOAPIC::isa_identity_map(int index)
    {
        InterruptDisabler disabler;

        configure_redirection_entry(index, InterruptManagement::acquire_mapped_interrupt_number(index) + IRQ_VECTOR_BASE, DeliveryMode::Normal, false, false, false, true, 0);
    }

    /// @breif: map_pci_interrupts
    void IOAPIC::map_pci_interrupts()
    {
        InterruptDisabler disabler;

        configure_redirection_entry(11, 11 + IRQ_VECTOR_BASE, DeliveryMode::Normal, false, false, true, true, 0);
    }

    /**
     * @return true 
     * @return false 
     */
    bool IOAPIC::is_enabled() const
    {
        return !is_hard_disabled();
    }

    /**
     * @param handler 
     */
    void IOAPIC::spurious_eoi(const GenericInterruptHandler& handler) const
    {
        InterruptDisabler disabler;

        ASSERT(handler.type() == HandlerType::SpuriousInterruptHandler);
        ASSERT(handler.interrupt_number() == APIC::spurious_interrupt_vector());

        klog() << "IOAPIC::spurious_eoi - Spurious Interrupt occurred";
    }

    void IOAPIC::map_isa_interrupts()
    {
        InterruptDisabler disabler;

        for (auto redirection_override : InterruptManagement::the().isa_overrides()) {
            if ((redirection_override.gsi() < gsi_base()) || (redirection_override.gsi() >= (gsi_base() + m_redirection_entries_count)))
                continue;

            bool active_low;

            switch ((redirection_override.flags() & 0b11)) {
            case 0:
                active_low = false;
                break;
            case 1:
                active_low = false;
                break;
            case 2:
                ASSERT_NOT_REACHED();
            case 3:
                active_low = true;
                break;
            }

            bool trigger_level_mode;

            switch (((redirection_override.flags() >> 2) & 0b11)) {
            case 0:
                trigger_level_mode = false;
                break;
            case 1:
                trigger_level_mode = false;
                break;
            case 2:
                ASSERT_NOT_REACHED();
            case 3:
                trigger_level_mode = true;
                break;
            }

            configure_redirection_entry(redirection_override.gsi() - gsi_base(), InterruptManagement::acquire_mapped_interrupt_number(redirection_override.source()) + IRQ_VECTOR_BASE, 0, false, active_low, trigger_level_mode, true, 0);
        }
    }

    /// @breif: rest all redirected entries
    void IOAPIC::reset_all_redirection_entries() const
    {
        InterruptDisabler disabler;

        for (size_t index = 0; index < m_redirection_entries_count; index++)
            reset_redirection_entry(index);
    }

    /// @brief: hard disable
    void IOAPIC::hard_disable()
    {
        InterruptDisabler disabler;

        reset_all_redirection_entries();
        IRQController::hard_disable();
    }   

    /**
     * @param index 
     */
    void IOAPIC::reset_redirection_entry(int index) const
    {
        InterruptDisabler disabler;
        configure_redirection_entry(index, 0, 0, false, false, false, true, 0);
    }

    /**
     * @param index 
     * @param interrupt_vector 
     * @param delivery_mode 
     * @param logical_destination 
     * @param active_low 
     * @param trigger_level_mode 
     * @param masked 
     * @param destination 
     */
    void IOAPIC::configure_redirection_entry(int index, u8 interrupt_vector, u8 delivery_mode, bool logical_destination, bool active_low, bool trigger_level_mode, bool masked, u8 destination) const
    {
        InterruptDisabler disabler;

        ASSERT((u32)index < m_redirection_entries_count);

        u32 redirection_entry1 = interrupt_vector | (delivery_mode & 0b111) << 8 | logical_destination << 11 | active_low << 13 | trigger_level_mode << 15 | masked << 16;
        u32 redirection_entry2 = destination << 24;

        write_register((index << 1) + IOAPIC_REDIRECTION_ENTRY_OFFSET, redirection_entry1);

    #ifdef IOAPIC_DEBUG
        dbg() << "IOAPIC Value: 0x" << String::format("%x", read_register((index << 1) + IOAPIC_REDIRECTION_ENTRY_OFFSET));
    #endif

        write_register((index << 1) + IOAPIC_REDIRECTION_ENTRY_OFFSET + 1, redirection_entry2);
        
    #ifdef IOAPIC_DEBUG
        dbg() << "IOAPIC Value: 0x" << String::format("%x", read_register((index << 1) + 0x11));
    #endif
    }

    /// @brief: mask all redirection entries
    void IOAPIC::mask_all_redirection_entries() const
    {
        InterruptDisabler disabler;
        for (size_t index = 0; index < m_redirection_entries_count; index++)
            mask_redirection_entry(index);
    }

    /**
     * @param index 
     */
    void IOAPIC::mask_redirection_entry(u8 index) const
    {
        ASSERT((u32)index < m_redirection_entries_count);

        u32 redirection_entry = read_register((index << 1) + IOAPIC_REDIRECTION_ENTRY_OFFSET);

        if (redirection_entry & (1 << 16))
            return;

        write_register((index << 1) + IOAPIC_REDIRECTION_ENTRY_OFFSET, redirection_entry | (1 << 16));
    }
    
    /**
     * @param index 
     * @return true 
     * @return false 
     */
    bool IOAPIC::is_redirection_entry_masked(u8 index) const
    {
        ASSERT((u32)index < m_redirection_entries_count);

        return (read_register((index << 1) + IOAPIC_REDIRECTION_ENTRY_OFFSET) & (1 << 16)) != 0;
    }

    /**
     * @param index 
     */
    void IOAPIC::unmask_redirection_entry(u8 index) const
    {
        ASSERT((u32)index < m_redirection_entries_count);
        u32 redirection_entry = read_register((index << 1) + IOAPIC_REDIRECTION_ENTRY_OFFSET);

        if (!(redirection_entry & (1 << 16)))
            return;

        write_register((index << 1) + IOAPIC_REDIRECTION_ENTRY_OFFSET, redirection_entry & ~(1 << 16));
    }

    /**
     * @param interrupt_vector 
     * @return true 
     * @return false 
     */
    bool IOAPIC::is_vector_enabled(u8 interrupt_vector) const
    {
        InterruptDisabler disabler;
        return is_redirection_entry_masked(interrupt_vector);
    }

    /**
     * @param index 
     * @return u8 
     */
    u8 IOAPIC::read_redirection_entry_vector(u8 index) const
    {
        ASSERT((u32)index < m_redirection_entries_count);
        return (read_register((index << 1) + IOAPIC_REDIRECTION_ENTRY_OFFSET) & 0xFF);
    }

    /**
     * @param vector 
     * @return Optional<int> 
     */
    Optional<int> IOAPIC::find_redirection_entry_by_vector(u8 vector) const
    {
        InterruptDisabler disabler;

        for (size_t index = 0; index < m_redirection_entries_count; index++) {
            if (read_redirection_entry_vector(index) == (InterruptManagement::acquire_mapped_interrupt_number(vector) + IRQ_VECTOR_BASE))
                return index;
        }
        
        return {};
    }

    /**
     * @param handler 
     */
    void IOAPIC::disable(const GenericInterruptHandler& handler)
    {
        InterruptDisabler disabler;
        ASSERT(!is_hard_disabled());
        u8 interrupt_vector = handler.interrupt_number();
        ASSERT(interrupt_vector >= gsi_base() && interrupt_vector < interrupt_vectors_count());
        auto found_index = find_redirection_entry_by_vector(interrupt_vector);

        if (!found_index.has_value()) {
            map_interrupt_redirection(interrupt_vector);
            found_index = find_redirection_entry_by_vector(interrupt_vector);
        }

        ASSERT(found_index.has_value());
        mask_redirection_entry(found_index.value());
    }

    /**
     * @param handler 
     */
    void IOAPIC::enable(const GenericInterruptHandler& handler)
    {
        InterruptDisabler disabler;
        ASSERT(!is_hard_disabled());
        u8 interrupt_vector = handler.interrupt_number();
        ASSERT(interrupt_vector >= gsi_base() && interrupt_vector < interrupt_vectors_count());
        auto found_index = find_redirection_entry_by_vector(interrupt_vector);

        if (!found_index.has_value()) {
            map_interrupt_redirection(interrupt_vector);
            found_index = find_redirection_entry_by_vector(interrupt_vector);
        }

        ASSERT(found_index.has_value());
        unmask_redirection_entry(found_index.value());
    }

    /**
     * @param handler 
     */
    void IOAPIC::eoi(const GenericInterruptHandler& handler) const
    {
        InterruptDisabler disabler;
        ASSERT(!is_hard_disabled());
        ASSERT(handler.interrupt_number() >= gsi_base() && handler.interrupt_number() < interrupt_vectors_count());
        ASSERT(handler.type() != HandlerType::SpuriousInterruptHandler);
        APIC::the().eoi();
    }

    /**
     * @return u16 
     */
    u16 IOAPIC::get_isr() const
    {
        InterruptDisabler disabler;
        ASSERT_NOT_REACHED();
    }

    /**
     * @return u16 
     */
    u16 IOAPIC::get_irr() const
    {
        InterruptDisabler disabler;
        ASSERT_NOT_REACHED();
    }

    /**
     * @param index 
     * @param value 
     */
    void IOAPIC::write_register(u32 index, u32 value) const
    {
        InterruptDisabler disabler;
        m_regs->select = index;
        m_regs->window = value;
    #ifdef IOAPIC_DEBUG
        dbg() << "IOAPIC writing.., Value 0x" << String::format("%x", m_regs->window) << " @ offset 0x" << String::format("%x", m_regs->select);
    #endif
    }

    /**
     * @param index 
     * @return u32 
     */
    u32 IOAPIC::read_register(u32 index) const
    {
        InterruptDisabler disabler;
        m_regs->select = index;
    #ifdef IOAPIC_DEBUG
        dbg() << "IOAPIC Reading, Value 0x" << String::format("%x", m_regs->window) << " @ offset 0x" << String::format("%x", m_regs->select);
    #endif
        return m_regs->window;
    }
}
