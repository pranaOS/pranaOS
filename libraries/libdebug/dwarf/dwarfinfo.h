/**
 * @file dwarfinfo.h
 * @author Krisna Pranav
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */


#pragma once

#include "attributevalue.h"
#include "compilationunit.h"
#include "dwarftypes.h"
#include <mods/bytebuffer.h>
#include <mods/nonnullownptrvector.h>
#include <mods/nonnullrefptr.h>
#include <mods/redblacktree.h>
#include <mods/refcounted.h>
#include <mods/string.h>
#include <libelf/image.h>

namespace Debug::Dwarf 
{

    class DwarfInfo 
    {
        MOD_MAKE_NONCOPYABLE(DwarfInfo);
        MOD_MAKE_NONMOVABLE(DwarfInfo);

    public:
        /**
         * @brief Construct a new Dwarf Info object
         * 
         */
        explicit DwarfInfo(ELF::Image const&);

        /**
         * @return ReadonlyBytes 
         */
        ReadonlyBytes debug_info_data() const 
        { 
            return m_debug_info_data; 
        }

        ReadonlyBytes abbreviation_data() const 
        { 
            return m_abbreviation_data; 
        }

        ReadonlyBytes debug_strings_data() const 
        { 
            return m_debug_strings_data; 
        }

        ReadonlyBytes debug_line_strings_data() const 
        { 
            return m_debug_line_strings_data; 
        }

        ReadonlyBytes debug_range_lists_data() const 
        { 
            return m_debug_range_lists_data; 
        }

        ReadonlyBytes debug_str_offsets_data() const 
        { 
            return m_debug_str_offsets_data; 
        }
        
        ReadonlyBytes debug_addr_data() const 
        { 
            return m_debug_addr_data; 
        }

        ReadonlyBytes debug_ranges_data() const 
        { 
            return m_debug_ranges_data; 
        }

        /**
         * @tparam Callback 
         */
        template<typename Callback>
        void for_each_compilation_unit(Callback) const;

        /**
         * @brief Get the attribute value object
         * 
         * @param form 
         * @param implicit_const_value 
         * @param debug_info_stream 
         * @param unit 
         * @return AttributeValue 
         */
        AttributeValue get_attribute_value(AttributeDataForm form, ssize_t implicit_const_value,
            InputMemoryStream& debug_info_stream, CompilationUnit const* unit = nullptr) const;

        /**
         * @brief Get the die at address object
         * 
         * @return Optional<DIE> 
         */
        Optional<DIE> get_die_at_address(FlatPtr) const;

        /**
         * @brief Get the cached die at offset object
         * 
         * @return Optional<DIE> 
         */
        Optional<DIE> get_cached_die_at_offset(FlatPtr) const;

    private:
        void populate_compilation_units();
        void build_cached_dies() const;

        /**
         * @param section_name 
         * @return ReadonlyBytes 
         */
        ReadonlyBytes section_data(StringView section_name) const;

        ELF::Image const& m_elf;
        ReadonlyBytes m_debug_info_data;
        ReadonlyBytes m_abbreviation_data;
        ReadonlyBytes m_debug_strings_data;
        ReadonlyBytes m_debug_line_data;
        ReadonlyBytes m_debug_line_strings_data;
        ReadonlyBytes m_debug_range_lists_data;
        ReadonlyBytes m_debug_str_offsets_data;
        ReadonlyBytes m_debug_addr_data;
        ReadonlyBytes m_debug_ranges_data;

        NonnullOwnPtrVector<Dwarf::CompilationUnit> m_compilation_units;

        struct DIERange {
            FlatPtr start_address { 0 };
            FlatPtr end_address { 0 };
        }; // struct DIERange

        struct DIEAndRange {
            DIE die;
            DIERange range;
        }; // struct DIEAndRange

        using DIEStartAddress = FlatPtr;

        mutable RedBlackTree<DIEStartAddress, DIEAndRange> m_cached_dies_by_range;
        mutable RedBlackTree<FlatPtr, DIE> m_cached_dies_by_offset;
        mutable bool m_built_cached_dies { false };
    }; // class DwarfInfo

    /**
     * @tparam Callback 
     * @param callback 
     */
    template<typename Callback>
    void DwarfInfo::for_each_compilation_unit(Callback callback) const
    {
        for (auto const& unit : m_compilation_units) {
            callback(unit);
        }
    }

} // namespace Debug::Dwarf
