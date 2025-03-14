/**
 * @file multiprocessorparser.h
 * @author Krisna Pranav
 * @brief multi processor parser
 * @version 6.0
 * @date 2023-08-17
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/types.h>
#include <kernel/physical_address.h>
#include <kernel/vm/region.h>
#include <kernel/virtual_address.h>

namespace Kernel 
{
    namespace MultiProcessor 
    {

        struct [[gnu::packed]] FloatingPointer
        {
            char sig[4];
            u32 physical_address_ptr;
            u8 length;
            u8 specification_revision;
            u8 checksum;
            u8 feature_info[5];
        }; // struct FloatingPointer

        struct [[gnu::packed]] EntryHeader
        {
            u8 entry_type;
        }; // struct EntryHeader

        struct [[gnu::packed]] ConfigurationTableHeader
        {
            char sig[4];
            u16 length;
            u8 specification_revision;
            u8 checksum;
            char oem_id[8];
            char product_id[12];
            u32 oem_table_ptr;
            u16 oem_table_size;
            u16 entry_count;
            u32 local_apic_address;
            u16 ext_table_length;
            u8 ext_table_checksum;
            u8 reserved;
            EntryHeader entries[];
        }; // struct ConfigurationTableHeader

        enum class ConfigurationTableEntryType 
        {
            Processor = 0,
            Bus = 1,
            IOAPIC = 2,
            IO_Interrupt_Assignment = 3,
            Local_Interrupt_Assignment = 4,
            SystemAddressSpaceMapping = 128,
            BusHierarchyDescriptor = 129,
            CompatibilityBusAddressSpaceModifier = 130
        }; // enum

        struct [[gnu::packed]] ExtEntryHeader
        {
            u8 entry_type;
            u8 entry_length;
        }; // struct ExtEntryHeader

        struct [[gnu::packed]] ProcessorEntry
        {
            EntryHeader h;
            u8 local_apic_id;
            u8 local_apic_version;
            u8 cpu_flags;
            u32 cpu_signature;
            u32 feature_flags;
            u8 reserved[8];
        }; // struct ProcessorEntry

        struct [[gnu::packed]] BusEntry
        {
            EntryHeader h;
            u8 bus_id;
            char bus_type[6];
        }; // struct BusEntry

        struct [[gnu::packed]] IOAPICEntry
        {
            EntryHeader h;
            u8 ioapic_id;
            u8 ioapic_version;
            u8 ioapic_flags;
            u32 ioapic_address;
        }; // struct IOAPICEntry

        enum class InterruptType 
        {
            INT = 0,
            NMI = 1,
            SMI = 2,
            ExtINT = 3,
        }; // enum InterruptType

        struct [[gnu::packed]] IOInterruptAssignmentEntry
        {
            EntryHeader h;
            u8 interrupt_type;
            u8 polarity;
            u8 trigger_mode;
            u8 source_bus_id;
            u8 source_bus_irq;
            u8 destination_ioapic_id;
            u8 destination_ioapic_intin_pin;
        }; // struct IOInterruptAssignmentEntry

        struct [[gnu::packed]] LocalInterruptAssignmentEntry
        {
            EntryHeader h;
            u8 interrupt_type;
            u8 polarity;
            u8 trigger_mode;
            u8 source_bus_id;
            u8 source_bus_irq;
            u8 destination_lapic_id;
            u8 destination_lapic_lintin_pin;
        }; // struct LocalInterruptAssignmentEntry

        enum class SystemAddressType 
        {
            IO = 0,
            Memory = 1,
            Prefetch = 2,
        }; // enum SystemAddressType

        struct [[gnu::packed]] SystemAddressSpaceMappingEntry
        {
            ExtEntryHeader h;
            u8 bus_id;
            u8 address_type;
            u64 address_base;
            u64 length;
        }; // struct SystemAddressSpaceMappingEntry

        struct [[gnu::packed]] BusHierarchyDescriptorEntry
        {
            ExtEntryHeader h;
            u8 bus_id;
            u8 bus_info;
            u8 parent_bus;
            u8 reserved[3];
        }; // struct BusHierarchyDescriptorEntry

        struct [[gnu::packed]] CompatibilityBusAddressSpaceModifierEntry
        {
            ExtEntryHeader h;
            u8 bus_id;
            u8 address_modifier;
            u32 predefined_range_list;
        }; // struct CompatibilityBusAddressSpaceModifierEntry

    } // namespace MultiProcessor

    class PCIInterruptOverrideMetadata;

    class MultiProcessorParser final 
    {
    public:
        /**
         * @return OwnPtr<MultiProcessorParser> 
         */
        static OwnPtr<MultiProcessorParser> autodetect();

        /**
         * @return Vector<PCIInterruptOverrideMetadata> 
         */
        Vector<PCIInterruptOverrideMetadata> get_pci_interrupt_redirections();

    private:

        /* 
         * @param floating_pointer 
         */
        explicit MultiProcessorParser(PhysicalAddress floating_pointer);

        void parse_configuration_table();
        void parse_floating_pointer_data();

        /**
         * @return Vector<u8> 
         */
        Vector<u8> get_pci_bus_ids() const;

        /**
         * @return Optional<PhysicalAddress> 
         */
        static Optional<PhysicalAddress> find_floating_pointer();

        PhysicalAddress m_floating_pointer;
        PhysicalAddress m_configuration_table;

        Vector<MultiProcessor::IOInterruptAssignmentEntry> m_io_interrupt_assignment_entries;
        
        Vector<MultiProcessor::BusEntry> m_bus_entries;

    }; // class MultiProcessorParser

} // namespace Kernel
