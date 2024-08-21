/**
 * @file mappedrom.h
 * @author Krisna Pranav
 * @brief mappedrom
 * @version 6.0
 * @date 2023-08-08
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
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

        PhysicalAddress paddr;

        /**
         * @param prefix 
         * @param chunk_size 
         * @return Optional<PhysicalAddress> 
         */
        Optional<PhysicalAddress> find_chunk_starting_with(StringView prefix, size_t chunk_size) const
        {
            for (auto* candidate = base(); candidate < end(); candidate += chunk_size) {
                if (!__builtin_memcmp(prefix.characters_without_null_termination(), candidate, prefix.length()))
                    return paddr_of(candidate);
            }

            return {};
        }

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
