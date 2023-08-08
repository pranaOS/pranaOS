/**
 * @file mappedrom.h
 * @author Krisna Pranav
 * @brief mappedrom
 * @version 1.0
 * @date 2023-08-08
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/ownptr.h>
#include <kernel/vm/region.h>

namespace Kernel
{
    class MappedROM
    {
    public:
        /**
         * @return const u8* 
         */
        const u8* base() const
        {
            return region->vaddr().offset(offset).as_ptr();
        }

        /**
         * @return const u8* 
         */
        const u8* end() const
        {
            return base() + size;
        }

        OwnPtr<Region> region;

        size_t size { 0 };
        size_t offset { 0 };

        /**
         * @param ptr 
         * @return PhysicalAddress 
         */
        PhysicalAddress paddr_of(const u8* ptr) const 
        { 
            return paddr.offset(ptr - this->base()); 
        }

    }; // class MappedROM
} // namespace Kernel