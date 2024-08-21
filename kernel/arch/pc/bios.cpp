/**
 * @file bios.cpp
 * @author Krisna Pranav aka (krishpranav)
 * @brief bios
 * @version 6.0
 * @date 2023-08-17
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/string_view.h>
#include <kernel/vm/memorymanager.h>
#include <kernel/vm/typedmapping.h>
#include <kernel/arch/pc/bios.h>

namespace Kernel 
{

    /**
     * @brief map_bios
     * 
     * @return MappedROM 
     */
    MappedROM map_bios()
    {
        MappedROM mapping;

        mapping.size = 128 * KiB;
        mapping.paddr = PhysicalAddress(0xe0000);
        mapping.region = MM.allocate_kernel_region(mapping.paddr, PAGE_ROUND_UP(mapping.size), {}, Region::Access::Read);
        
        return mapping;
    }

    /**
     * @brief map_ebda
     * 
     * @return MappedROM 
     */
    MappedROM map_ebda()
    {
        auto ebda_segment_ptr = map_typed<u16>(PhysicalAddress(0x40e));
        auto ebda_length_ptr = map_typed<u16>(PhysicalAddress(0x413));

        PhysicalAddress ebda_paddr(*ebda_segment_ptr << 4);
        size_t ebda_size = *ebda_length_ptr;

        MappedROM mapping;

        mapping.region = MM.allocate_kernel_region(ebda_paddr.page_base(), PAGE_ROUND_UP(ebda_size), {}, Region::Access::Read);
        mapping.offset = ebda_paddr.offset_in_page();
        mapping.size = ebda_size;
        mapping.paddr = ebda_paddr;

        return mapping;
    }

} // namespace Kernel
