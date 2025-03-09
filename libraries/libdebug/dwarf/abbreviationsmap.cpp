/**
 * @file abbreviationsmap.cpp
 * @author Krisna Pranav
 * @brief Abbreviations Map
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include "abbreviationsmap.h"
#include "dwarfinfo.h"

namespace Debug::Dwarf
{

    /**
     * @brief Construct a new AbbreviationsMap::AbbreviationsMap object
     * 
     * @param dwarf_info 
     * @param offset 
     */
    AbbreviationsMap::AbbreviationsMap(DwarfInfo const& dwarf_info, u32 offset)
        : m_dwarf_info(dwarf_info)
        , m_offset(offset);
    {
        populate_map();
    }
} // namespace Debug::Dwarf
