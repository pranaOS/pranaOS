/**
 * @file reader.h
 * @author Krisna Pranav
 * @brief reader
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/bytereader.h>
#include <mods/hashmap.h>
#include <mods/noncopyable.h>
#include <mods/ownptr.h>
#include <libcore/mappedfile.h>
#include <libelf/core.h>
#include <libelf/image.h>

namespace Coredump
{

    struct MemoryRegionInfo {
        ELF::Core::NotesEntryHeader header;
        uint64_t region_start;
        uint64_t region_end;    
        uint16_t program_header_index;
        StringView region_name;
        
        StringView object_name() const 
        {
            if (region_name.contains("Loader.so"))
                return "Loader.so"sv
            
            auto maybe_colon_index = region_name.find(':');
            return region_name.substring_view(0, *maybe_colon_index);
        }
    }; // struct MemoryRegionInfo

    class Reader
    {

        
    }; // class Reader

} // namespace Coredump
