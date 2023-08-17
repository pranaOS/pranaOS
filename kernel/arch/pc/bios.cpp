/**
 * @file bios.cpp
 * @author Krisna Pranav aka (krishpranav)
 * @brief bios
 * @version 1.0
 * @date 2023-08-17
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/string_view.h>
#include <kernel/vm/memorymanager.h>
#include <kernel/vm/typedmapping.h>
#include <kernel/arch/pc/bios.h>

namespace Kernel
{
    /**
     * @return MappedROM 
     */
    MappedROM map_bios()
    {
        MappedROM mapping;
        mapping.size = 128 * kib;
        mapping.paddr = PhysicalAddress(0xe0000);
        mapping.region = MM.allocate_kernel_region(mapping.paddr, PAGE_ROUND_UP);
        return mapping;
    } // map_bios

} // namespace Kernel