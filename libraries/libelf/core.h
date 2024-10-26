/**
 * @file core.h
 * @author Krisna Pranav
 * @brief 
 * @version 0.1
 * @date 2024-10-26
 * 
 * @copyright Copyright (c) 2024
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
            Metadata
        }; // enum Type : u8

        Type type;
        
    }; // struct [[gnu::packed]] NotesEntryHeader
} // namespace ELF::Core