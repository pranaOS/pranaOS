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

    public:
        enum class Type : u8 {
            UnsignedNumber,
            SignedNumber,
            String,
            DieReference, 
            Boolean,
            DwarfExpression,
            SecOffset,
            RawBytes,
            Address
        }; // enum class Type : u8

        /**
         * @return Type 
         */
        Type type() const 
        { 
            return m_type; 
        }

        /**
         * @return AttributeDataForm 
         */
        AttributeDataForm form() const 
        { 
            return m_form; 
        }

        /**
         * @return FlatPtr 
         */
        FlatPtr as_addr() const;

        /**
         * @return u64 
         */
        u64 as_unsigned() const 
        { 
            return m_data.as_unsigned; 
        }

        /**
         * @return i64 
         */
        i64 as_signed() const 
        { 
            return m_data.as_signed; 
        }

        char const* as_string() const;

        /**
         * @return true 
         * @return false 
         */
        bool as_bool() const 
        { 
            return m_data.as_bool; 
        }

        /**
         * @return ReadonlyBytes 
         */
        ReadonlyBytes as_raw_bytes() const 
        { 
            return m_data.as_raw_bytes; 
        }

    private:
        Type m_type;

        union {
            FlatPtr as_addr;
            u64 as_unsigned;
            i64 as_signed;
            char const* as_string; 
            bool as_bool;
            ReadonlyBytes as_raw_bytes;
        } m_data {};

        AttributeDataForm m_form {};

        CompilationUnit const* m_compilation_unit { nullptr };
    }; // class AttributeValue

} // namespace Debug::Dwarf
