/**
 * @file attributevalue.h
 * @author Krisna Pranav
 * @brief attribute value
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/span.h>
#include <mods/types.h>
#include <libdebug/dwarf/dwarftypes.h>

namespace Debug::Dwarf
{

    class CompilationUnit;

    class AttributeValue
    {
        friend class DwarfInfo;

    private:
        Type m_type;

        union {
            FlatPtr as_addr;
            u64 as_unsigned;
            i64 as_signed;
            char const* as_string;
            bool as_bool;
        } m_data {};

        AttributeDataForm m_form {};

        CompilationUnit const* m_compilation_unit { nullptr };
    }; // class AttributeValue
} // namespace Debug::Dwarf