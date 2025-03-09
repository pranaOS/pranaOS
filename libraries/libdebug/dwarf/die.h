/**
 * @file die.h
 * @author Krisna Pranav
 * @brief die
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

namespace Debug::Dwarf
{

    class CompilationUnit;

    class DIE
    {
    
    private:
        void rehydrate_from(u32 offset, Optional<u32> parent_offset);

        CompilationUnit const& m_compilation_unit;
        u32 m_offset { 0 };
        u32 m_data_offset { 0 };
        size_t m_abbreviation_code { 0 };
        EntryTag m_tag { EntryTag::None };
        bool m_has_children { false };
        u32 m_size { 0 };
        Optional<u32> m_parent_offset;
    }; // class DIE

} // namespace Debug::Dwarf
