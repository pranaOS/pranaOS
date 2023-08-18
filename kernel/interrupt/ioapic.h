/**
 * @file ioapic.h
 * @author Krisna Pranav
 * @brief ioapic
 * @version 6.0
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
        volatile u32 select;
        u32 reserved[3];
        volatile u32 window;
    };

    class PCIInterruptOverrideMetadata 
    {
    public:
        /**
         * @param bus_id 
         * @param polarity 
         * @param trigger_mode 
         * @param source_irq 
         * @param ioapic_id 
         * @param ioapic_int_pin 
         */
        PCIInterruptOverrideMetadata(u8 bus_id, u8 polarity, u8 trigger_mode, u8 source_irq, u32 ioapic_id, u16 ioapic_int_pin);

        /**
         * @return u8 
         */
        u8 bus() const 
        { 
            return m_bus_id; 
        }

        u8 polarity() const 
        { 
            return m_polarity; 
        }

        u8 trigger_mode() const 
        { 
            return m_trigger_mode; 
        }
        
        u8 pci_interrupt_pin() const 
        { 
            return m_pci_interrupt_pin; 
        }

        u8 pci_device_number() const 
        { 
            return m_pci_device_number; 
        }

        /**
         * @return u32 
         */
        u32 ioapic_id() const 
        { 
            return m_ioapic_id; 
        }

        /**
         * @return u16 
         */
        u16 ioapic_interrupt_pin() const 
        { 
            return m_ioapic_interrupt_pin; 
        }

    private:
    
        const u8 m_bus_id;
        const u8 m_polarity;
        const u8 m_trigger_mode;
        const u8 m_pci_interrupt_pin;
        const u8 m_pci_device_number;
        const u32 m_ioapic_id;
        const u16 m_ioapic_interrupt_pin;
    }; // class PCIInterruptOverrideMetadata

    class IOAPIC final : public IRQController 
    {
    public:
        /**
         * @param gsi_base 
         */
        IOAPIC(PhysicalAddress, u32 gsi_base);

        /// @brief: enable, disable
        virtual void enable(const GenericInterruptHandler&) override;
        virtual void disable(const GenericInterruptHandler&) override;

        /// @brief: hard_disable
        virtual void hard_disable() override;

        /// @brief: eoi
        virtual void eoi(const GenericInterruptHandler&) const override;
        
        /// @breif: spurious_eoi 
        virtual void spurious_eoi(const GenericInterruptHandler&) const override;

        /**
         * @param number 
         * @return true 
         * @return false 
         */
        virtual bool is_vector_enabled(u8 number) const override;

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_enabled() const override;

        /**
         * @return u16 
         */
        virtual u16 get_isr() const override;
        virtual u16 get_irr() const override;

        /**
         * @return u32 
         */
        virtual u32 gsi_base() const override 
        { 
            return m_gsi_base; 
        }

        /**
         * @return size_t 
         */
        virtual size_t interrupt_vectors_count() const 
        { 
            return m_redirection_entries_count; 
        }

        /**
         * @return const char* 
         */
        virtual const char* model() const override 
        { 
            return "IOAPIC"; 
        };

        /**
         * @return IRQControllerType 
         */
        virtual IRQControllerType type() const override 
        { 
            return IRQControllerType::i82093AA; 
        }

    private:
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
        void configure_redirection_entry(int index, u8 interrupt_vector, u8 delivery_mode, bool logical_destination, bool active_low, bool trigger_level_mode, bool masked, u8 destination) const;
        
        /**
         * @param index 
         */
        void reset_redirection_entry(int index) const;

        /**
         * @param interrupt_vector 
         */
        void map_interrupt_redirection(u8 interrupt_vector);

        void reset_all_redirection_entries() const;

        void mask_all_redirection_entries() const;

        /**
         * @param index 
         */
        void mask_redirection_entry(u8 index) const;

        /**
         * @param index 
         */
        void unmask_redirection_entry(u8 index) const;

        /**
         * @param index 
         * @return true 
         * @return false 
         */
        bool is_redirection_entry_masked(u8 index) const;

        /**
         * @param index 
         * @return u8 
         */
        u8 read_redirection_entry_vector(u8 index) const;

        /**
         * @param vector 
         * @return Optional<int> 
         */
        Optional<int> find_redirection_entry_by_vector(u8 vector) const;

        void configure_redirections() const;

        /**
         * @param index 
         * @param value 
         */
        void write_register(u32 index, u32 value) const;

        /**
         * @param index 
         * @return u32 
         */
        u32 read_register(u32 index) const;

        /// @breif: initialize
        virtual void initialize() override;

        void map_isa_interrupts();

        void map_pci_interrupts();

        /**
         * @param index 
         */
        void isa_identity_map(int index);

        PhysicalAddress m_address;

        mutable TypedMapping<ioapic_mmio_regs> m_regs;

        u32 m_gsi_base;
        u8 m_id;
        u8 m_version;

        size_t m_redirection_entries_count;
    }; // class IOAPIC

} // namespace Kernel
