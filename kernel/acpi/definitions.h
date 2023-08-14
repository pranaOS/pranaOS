/**
 * @file definitions.h
 * @author Krisna Pranav
 * @brief definitions
 * @version 1.0
 * @date 2023-08-14
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/physical_address.h>
#include <mods/refcounted.h>
#include <mods/types.h>
#include <mods/vector.h>

namespace Kernel
{
    namespace ACPI 
    {
            namespace FADTFlags
            {
                enum class FeatureFlags : u32 
                {

                }; // enum 

                class StaticParser;
                class DynamicParser;
                class Parser;

                /**
                 * @return Optional<PhysicalAddress> 
                 */
                Optional<PhysicalAddress> find_rsdp();  
                
                /**
                 * @param rsdp 
                 * @param signature 
                 * @return PhysicalAddress 
                 */
                PhysicalAddress find_table(PhysicalAddress rsdp, const StringView& signature);

            } // namespace FADTFlags
    } // namespace ACPI
} // namespace Kernel