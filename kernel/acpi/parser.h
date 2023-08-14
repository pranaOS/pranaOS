/**
 * @file parser.h
 * @author Krisna Pranav
 * @brief parser
 * @version 1.0
 * @date 2023-08-14
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/physical_address.h>
#include <kernel/virtual_address.h>
#include <kernel/acpi/definitions.h>
#include <kernel/acpi/initialize.h>
#include <kernel/filesystem/file.h>
#include <kernel/vm/region.h>
#include <mods/types.h>

namespace Kernel
{
    namespace ACPI 
    {
        class Parser
        {
        private:
            PhysicalAddress m_rsdp;
            PhysicalAddress m_main_system_description_table;
        }; // class Parser
    } // namespace ACPI 

} // namespace Kernel