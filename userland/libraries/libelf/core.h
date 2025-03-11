/**
 * @file core.h
 * @author Krisna Pranav
 * @brief 
 * @version 0.1
 * @date 2024-10-26
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>
#include <libc/sys/arch/i386/regs.h>

#ifndef KERNEL
#   include <mods/string.h>
#endif

namespace ELF::Core 
{
    struct [[gnu::packed]] NotesEntryHeader 
    {
        enum Type : u8 
        {
            Null = 0, 
            ProcessInfo,
            ThreadInfo,
            MemoryRegionInfo,
            Metadata,
        }; // enum Type : u8 
        Type type;
    }; // struct [[gnu::packed]] NotesEntryHeader 

    struct [[gnu::packed]] NotesEntry 
    {
        NotesEntryHeader header;
        char data[];
    }; // struct [[gnu::packed]] NotesEntry 

    struct [[gnu::packed]] ProcessInfo 
    {
        NotesEntryHeader header;
        char json_data[]; 
    }; // struct [[gnu::packed]] ProcessInfo 

    struct [[gnu::packed]] ThreadInfo 
    {
        NotesEntryHeader header;
        int tid;
        PtraceRegisters regs;
    }; // struct [[gnu::packed]] ThreadInfo 

    struct [[gnu::packed]] MemoryRegionInfo
    {
        NotesEntryHeader header;
        uint64_t region_start;
        uint64_t region_end;
        uint16_t program_header_index;
        char region_name[]; 

    #ifndef KERNEL
        String object_name() const
        {
            StringView memory_region_name { region_name };
            if (memory_region_name.contains("Loader.so"))
                return "Loader.so";
            auto maybe_colon_index = memory_region_name.find(':');
            if (!maybe_colon_index.has_value())
                return {};
            return memory_region_name.substring_view(0, *maybe_colon_index).to_string();
        }
    #endif
    }; // struct [[gnu::packed]] MemoryRegionInfo

    struct [[gnu::packed]] Metadata 
    {
        NotesEntryHeader header;
        char json_data[]; 
    }; // struct [[gnu::packed]] Metadata 
} // namespace ELF::Core
