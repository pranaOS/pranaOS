/**
 * @file coredump.h
 * @author Krisna Pranav
 * @brief coredump
 * @version 0.1
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <libc/sys/arch/i386/regs.h>
#include <mods/types.h>

namespace ELF::Core 
{

    struct [[gnu::packed]] NotesEntryHeader
    {
        enum Type : u8 
        {
            Null = 0, 
            ThreadInfo,
            MemoryRegionInfo,
        };

        Type type;
    }; // struct

    struct [[gnu::packed]] NotesEntry
    {
        NotesEntryHeader header;
        char data[];
    }; // struct

    struct [[gnu::packed]] ThreadInfo
    {
        NotesEntryHeader header;
        int tid;
        PtraceRegisters regs;
    }; // struct

    struct [[gnu::packed]] MemoryRegionInfo
    {
        NotesEntryHeader header;
        uint32_t region_start;
        uint32_t region_end;
        uint16_t program_header_index;
        char region_name[];
    }; // struct

}