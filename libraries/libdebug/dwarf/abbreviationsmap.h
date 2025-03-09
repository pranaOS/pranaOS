/**
 * @file abbreviationsmap.h
 * @author Krisna Pranav
 * @brief Abbreviations Map
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */


#pragma once

#include "dwarftypes.h"
#include <mods/hashmap.h>
#include <mods/optional.h>
#include <mods/types.h>

namespace Debug::Dwarf 
{

    class DwarfInfo;

    class AbbreviationsMap 
    {
    public:
        /**
         * @brief Construct a new Abbreviations Map object
         * 
         * @param dwarf_info 
         * @param offset 
         */
        AbbreviationsMap(DwarfInfo const& dwarf_info, u32 offset);

        struct AbbreviationEntry {
            EntryTag tag;
            bool has_children;

            Vector<AttributeSpecification> attribute_specifications;
        }; // struct AbbreviationEntry

        /**
         * @param code 
         * @return AbbreviationEntry const* 
         */
        AbbreviationEntry const* get(u32 code) const;

    private:
        void populate_map();

        DwarfInfo const& m_dwarf_info;
        u32 m_offset { 0 };
        HashMap<u32, AbbreviationEntry> m_entries;
    }; // class AbbreviationsMap

} // namespace Debug::Dwarf
