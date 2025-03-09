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

#include "attributevalue.h"
#include "dwarftypes.h"
#include <mods/function.h>
#include <mods/nonnullownptr.h>
#include <mods/optional.h>
#include <mods/types.h>

namespace Debug::Dwarf 
{

    class CompilationUnit;

    class DIE 
    {
    public:
        /**
         * @brief Construct a new DIE object
         * 
         * @param offset 
         * @param parent_offset 
         */
        DIE(CompilationUnit const&, u32 offset, Optional<u32> parent_offset = {});

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
            return m_size; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool has_children() const 
        { 
            return m_has_children; 
        }

        /**
         * @return EntryTag 
         */
        EntryTag tag() const 
        { 
            return m_tag; 
        }

        /**
         * @brief Get the attribute object
         * 
         * @return Optional<AttributeValue> 
         */
        Optional<AttributeValue> get_attribute(Attribute const&) const;

        /**
         * @param callback 
         */
        void for_each_child(Function<void(DIE const& child)> callback) const;

        /**
         * @return true 
         * @return false 
         */
        bool is_null() const 
        { 
            return m_tag == EntryTag::None; 
        }

        /**
         * @return CompilationUnit const& 
         */
        CompilationUnit const& compilation_unit() const 
        { 
            return m_compilation_unit; 
        }

        /**
         * @return Optional<u32> 
         */
        Optional<u32> parent_offset() const 
        { 
            return m_parent_offset; 
        }

    private:
        /**
         * @param offset 
         * @param parent_offset 
         */
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
