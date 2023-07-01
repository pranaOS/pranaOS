/**
 * @file format.h
 * @author Krisna Pranav
 * @brief format
 * @version 1.0
 * @date 2023-07-01
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "array.h"
#include "genericlexer.h"
#include "string_view.h"

#ifdef KERNEL
#   include <stdio.h>
#endif

namespace Mods {
    class TypeErasedFormatParams;
    class FormatParser;
    class FormatBuilder;

    template<typename T, typename = void>
    struct Formatter {
        using __no_formatter_defined = void;
    };

    constexpr size_t max_format_arguments = 256;

    struct TypeErrasedParameter {
        enum class Type {
            UInt8,
            UInt16,
        };

        template<typename T>
        static Type get_type() {
            if (IsSame<T, u8>::value)
                return Type::UInt8;
        }

        const void* value;
        void(*formatter)(TypeErrasedParameter);
    };
}