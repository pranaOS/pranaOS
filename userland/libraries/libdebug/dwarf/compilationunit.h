/**
 * @file compilationunit.h
 * @author Krisna Pranav
 * @brief Compilation Unit
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */


#pragma once

#include "abbreviationsmap.h"
#include "die.h"
#include "lineprogram.h"
#include <mods/noncopyable.h>
#include <mods/types.h>

namespace Debug::Dwarf 
{

    class DwarfInfo;
    class DIE;
    class LineProgram;

    class CompilationUnit 
    {
        MOD_MAKE_NONCOPYABLE(CompilationUnit);
        MOD_MAKE_NONMOVABLE(CompilationUnit);

    public:
        /**
         * @brief Construct a new Compilation Unit object
         * 
         * @param dwarf_info 
         * @param offset 
         * @param line_program 
         */
        CompilationUnit(DwarfInfo const& dwarf_info, u32 offset, CompilationUnitHeader const&, NonnullOwnPtr<LineProgram>&& line_program);

        /**
         * @return u32 
         */
        u32 offset() const 
        { 
            return m_offset; 
        }

        /**
         * @return u32 
         */
        u32 size() const 
        { 
            return m_header.length() + sizeof(u32); 
        }

        /**
         * @return DIE 
         */
        DIE root_die() const;
        DIE get_die_at_offset(u32 offset) const;

        /**
         * @brief Get the address object
         * 
         * @param index 
         * @return FlatPtr 
         */
        FlatPtr get_address(size_t index) const;

        /**
         * @brief Get the string object
         * 
         * @param index 
         * @return char const* 
         */
        char const* get_string(size_t index) const;

        /**
         * @return u8 
         */
        u8 dwarf_version() const 
        { 
            return m_header.version(); 
        }

        /**
         * @return DwarfInfo const& 
         */
        DwarfInfo const& dwarf_info() const 
        { 
            return m_dwarf_info; 
        }

        /**
         * @return AbbreviationsMap const& 
         */
        AbbreviationsMap const& abbreviations_map() const 
        { 
            return m_abbreviations; 
        }

        /**
         * @return LineProgram const& 
         */
        LineProgram const& line_program() const 
        { 
            return *m_line_program; 
        }

        /**
         * @return Optional<FlatPtr> 
         */
        Optional<FlatPtr> base_address() const;

        u64 address_table_base() const;

        u64 string_offsets_base() const;

        u64 range_lists_base() const;

    private:
        DwarfInfo const& m_dwarf_info;
        u32 m_offset { 0 };
        CompilationUnitHeader m_header;
        AbbreviationsMap m_abbreviations;
        NonnullOwnPtr<LineProgram> m_line_program;
        mutable bool m_has_cached_base_address : 1 { false };
        mutable bool m_has_cached_address_table_base : 1 { false };
        mutable bool m_has_cached_string_offsets_base : 1 { false };
        mutable bool m_has_cached_range_lists_base : 1 { false };
        mutable Optional<FlatPtr> m_cached_base_address;
        mutable u64 m_cached_address_table_base { 0 };
        mutable u64 m_cached_string_offsets_base { 0 };
        mutable u64 m_cached_range_lists_base { 0 };
    }; // class CompilationUnit

} // namespace Debug::Dwarf
