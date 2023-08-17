/**
 * @file multiprocessorparser.h
 * @author Krisna Pranav
 * @brief multi processor parser
 * @version 1.0
 * @date 2023-08-17
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
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
        enum class SystemAddressType
        {
            IO = 0,
            Memory = 1,
            Prefetch = 2,
        };

        class PCIInterruptOverrideMetadata;

        class MultiProcessorParser final
        {
        public:
            static OwnPtr<MultiProcessorParser> autodetect();

            Vector<PCIInterruptOverrideMetadata> get_pci_interrupt_redirections();

        private:
            explicit MultiProcessorParser(PhysicalAddress floating_pointer);

            void parse_configuration_table();
            void parse_floating_pointer_data();

            Vector<u8> get_pci_bus_ids() const;

            static Optional<PhysicalAddress> find_floating_pointer();
        }; // class MultiProcessorParser
        
    } // namespace MultiProcessor

} // namespace Kernel