/**
 * @file lineprogram.h
 * @author Krisna Pranav
 * @brief line program
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "dwarfinfo.h"
#include <mods/flystring.h>
#include <mods/memorystream.h>
#include <mods/vector.h>

namespace Debug::Dwarf
{

    enum class ContentType {
        Path = 1,
        DirectoryIndex = 2,
        Timestamp = 3,
        Size = 4,
        MD5 = 5,
        LoUser = 0x2000,
        HiUser = 0x3fff,
    }; // enum class ContentType

    struct PathEntryFormat {
        ContentType type;
        AttributeDataForm form;
    }; // struct PathEntryFormat

    struct PathEntry {
        String path;
        size_t directory_index { 0 };
    }; // struct PathEntry

    enum class PathListType {
        Directories,
        Filenames,
    }; // enum class PathListType

    /**
     * @param stream 
     * @param header 
     * @return InputStream& 
     */
    inline InputStream& operator>>(InputStream& stream, LineProgramUnitHeader32& header)
    {
        stream.read_or_error(Bytes { &header.common, sizeof(header.common) });
        if (header.common.version <= 4)
            stream.read_or_error(Bytes { &header.v4, sizeof(header.v4) });
        else
            stream.read_or_error(Bytes { &header.v5, sizeof(header.v5) });
        stream.read_or_error(Bytes { &header.std_opcode_lengths, min(sizeof(header.std_opcode_lengths), (header.opcode_base() - 1) * sizeof(header.std_opcode_lengths[0])) });
        return stream;
    }

} // namespace Debug::Dwarf
