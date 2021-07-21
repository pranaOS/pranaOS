/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <base/CheckedFormatString.h>

#include <base/AllOf.h>
#include <base/AnyOf.h>
#include <base/Array.h>
#include <base/GenericLexer.h>
#include <base/Optional.h>
#include <base/StringView.h>

#ifndef KERNEL
#    include <stdio.h>
#endif

namespace Base {

class TypeErasedFormatParams;
class FormatParser;
class FormatBuilder;

template<typename T, typename = void>
struct Formatter {
    using __no_formatter_defined = void;
};

template<typename T, typename = void>
inline constexpr bool HasFormatter = true;

template<typename T>
inline constexpr bool HasFormatter<T, typename Formatter<T>::__no_formatter_defined> = false;

constexpr size_t max_format_arguments = 256;

struct TypeErasedParameter {
    enum class Type {
        UInt8,
        UInt16,
        UInt32,
        UInt64,
        Int8,
        Int16,
        Int32,
        Int64,
        Custom
    };

    template<size_t size, bool is_unsigned>
    static consteval Type get_type_from_size()
    {
        if constexpr (is_unsigned) {
            if constexpr (size == 1)
                return Type::UInt8;
            if constexpr (size == 2)
                return Type::UInt16;
            if constexpr (size == 4)
                return Type::UInt32;
            if constexpr (size == 8)
                return Type::UInt64;
        } else {
            if constexpr (size == 1)
                return Type::Int8;
            if constexpr (size == 2)
                return Type::Int16;
            if constexpr (size == 4)
                return Type::Int32;
            if constexpr (size == 8)
                return Type::Int64;
        }

        VERIFY_NOT_REACHED();
    }

    template<typename T>
    static consteval Type get_type()
    {
        if constexpr (IsIntegral<T>)
            return get_type_from_size<sizeof(T), IsUnsigned<T>>();
        else
            return Type::Custom;
    }

    constexpr size_t to_size() const
    {
        i64 svalue;

        if (type == TypeErasedParameter::Type::UInt8)
            svalue = *static_cast<const u8*>(value);
        else if (type == TypeErasedParameter::Type::UInt16)
            svalue = *static_cast<const u16*>(value);
        else if (type == TypeErasedParameter::Type::UInt32)
            svalue = *static_cast<const u32*>(value);
        else if (type == TypeErasedParameter::Type::UInt64)
            svalue = *static_cast<const u64*>(value);
        else if (type == TypeErasedParameter::Type::Int8)
            svalue = *static_cast<const i8*>(value);
        else if (type == TypeErasedParameter::Type::Int16)
            svalue = *static_cast<const i16*>(value);
        else if (type == TypeErasedParameter::Type::Int32)
            svalue = *static_cast<const i32*>(value);
        else if (type == TypeErasedParameter::Type::Int64)
            svalue = *static_cast<const i64*>(value);
        else
            VERIFY_NOT_REACHED();

        VERIFY(svalue >= 0);

        return static_cast<size_t>(svalue);
    }


    const void* value;
    Type type;
    void (*formatter)(TypeErasedFormatParams&, FormatBuilder&, FormatParser&, const void* value);
};

class FormatParser : public GenericLexer {
public:
    struct FormatSpecifier {
        StringView flags;
        size_t index;
    };

    explicit FormatParser(StringView input);

    StringView consume_literal();
    bool consume_number(size_t& value);
    bool consume_specifier(FormatSpecifier& specifier);
    bool consume_replacement_field(size_t& inedx);

};

}