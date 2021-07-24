/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/StringView.h>
#include <kernel/ACPI/MultiProcessorParser.h>
#include <kernel/Arch/PC/BIOS.h>
#include <kernel/Debug.h>
#include <kernel/Interrupts/IOAPIC.h>
#include <kernel/Sections.h>
#include <kernel/StdLib.h>
#include <kernel/VM/TypedMapping.h>

namespace Kernel {

UNMAP_AFTER_INIT OwnPtr<MultiProcessorParser> MultiProcessorParser::autodetect()
{
    auto floating_pointer = find_floating_pointer();
    if (!floating_pointer.has_value())
        return {};
    auto parser = adopt_own_if_nonnull(new (nothrow) MultiProcessorParser(floating_pointer.value()));
    VERIFY(parser != nullptr);
    return parser;
}

UNMAP_AFTER_INIT MultiProcessorParser::MultiProcessorParser(PhysicalAddress floating_pointer)
    : m_floating_pointer(floating_pointer)
{
    dbgln("MultiProcessor: Floating Pointer Structure @ {}", m_floating_pointer);
    parse_floating_pointer_data();
    parse_configuration_table();
}

UNMAP_AFTER_INIT void MultiProcessorParser::parse_floating_pointer_data()
{
    auto floating_pointer = map_typed<MultiProcessor::FloatingPointer>(m_floating_pointer);
    m_configuration_table = PhysicalAddress(floating_pointer->physical_address_ptr);
    dbgln("Features {}, IMCR? {}", floating_pointer->feature_info[0], (floating_pointer->feature_info[0] & (1 << 7)));
}

UNMAP_AFTER_INIT void MultiProcessorParser::parse_configuration_table()
{
    auto configuration_table_length = map_typed<MultiProcessor::ConfigurationTableHeader>(m_configuration_table)->length;
    auto config_table = map_typed<MultiProcessor::ConfigurationTableHeader>(m_configuration_table, configuration_table_length);

    size_t entry_count = config_table->entry_count;
    auto* entry = config_table->entries;
    while (entry_count > 0) {
        dbgln_if(MULTIPROCESSOR_DEBUG, "MultiProcessor: Entry Type {} detected.", entry->entry_type);
        switch (entry->entry_type) {
        case ((u8)MultiProcessor::ConfigurationTableEntryType::Processor):
            entry = (MultiProcessor::EntryHeader*)(FlatPtr)entry + sizeof(MultiProcessor::ProcessorEntry);
            break;
        case ((u8)MultiProcessor::ConfigurationTableEntryType::Bus):
            m_bus_entries.append(*(const MultiProcessor::BusEntry*)entry);
            entry = (MultiProcessor::EntryHeader*)(FlatPtr)entry + sizeof(MultiProcessor::BusEntry);
            break;
        case ((u8)MultiProcessor::ConfigurationTableEntryType::IOAPIC):
            entry = (MultiProcessor::EntryHeader*)(FlatPtr)entry + sizeof(MultiProcessor::IOAPICEntry);
            break;
        case ((u8)MultiProcessor::ConfigurationTableEntryType::IO_Interrupt_Assignment):
            m_io_interrupt_assignment_entries.append(*(const MultiProcessor::IOInterruptAssignmentEntry*)entry);
            entry = (MultiProcessor::EntryHeader*)(FlatPtr)entry + sizeof(MultiProcessor::IOInterruptAssignmentEntry);
            break;
        case ((u8)MultiProcessor::ConfigurationTableEntryType::Local_Interrupt_Assignment):
            entry = (MultiProcessor::EntryHeader*)(FlatPtr)entry + sizeof(MultiProcessor::LocalInterruptAssignmentEntry);
            break;
        case ((u8)MultiProcessor::ConfigurationTableEntryType::SystemAddressSpaceMapping):
            entry = (MultiProcessor::EntryHeader*)(FlatPtr)entry + sizeof(MultiProcessor::SystemAddressSpaceMappingEntry);
            break;
        case ((u8)MultiProcessor::ConfigurationTableEntryType::BusHierarchyDescriptor):
            entry = (MultiProcessor::EntryHeader*)(FlatPtr)entry + sizeof(MultiProcessor::BusHierarchyDescriptorEntry);
            break;
        case ((u8)MultiProcessor::ConfigurationTableEntryType::CompatibilityBusAddressSpaceModifier):
            entry = (MultiProcessor::EntryHeader*)(FlatPtr)entry + sizeof(MultiProcessor::CompatibilityBusAddressSpaceModifierEntry);
            break;
        default:
            VERIFY_NOT_REACHED();
        }
        --entry_count;
    }
}

}