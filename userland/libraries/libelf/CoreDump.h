/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/String.h>
#include <base/Types.h>
#include <libc/sys/arch/i386/regs.h>

namespace ELF::Core {

struct [[gnu::packed]] NotesEntryHeader {
    enum Type : u8 {
        Null = 0, 
        ProcessInfo,
        ThreadInfo,
        MemoryRegionInfo,
        Metadata,
    };
    Type type;
};

struct [[gnu::packed]] NotesEntry {
    NotesEntryHeader header;
    char data[];
};

struct [[gnu::packed]] ProcessInfo {
    NotesEntryHeader header;
    char json_data[]; 
};

struct [[gnu::packed]] ThreadInfo {
    NotesEntryHeader header;
    int tid;
    PtraceRegisters regs;
};

struct [[gnu::packed]] MemoryRegionInfo {
    NotesEntryHeader header;
    uint64_t region_start;
    uint64_t region_end;
    uint16_t program_header_index;
    char region_name[]; 

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
};

struct [[gnu::packed]] Metadata {
    NotesEntryHeader header;

    char json_data[]; 
};

}
