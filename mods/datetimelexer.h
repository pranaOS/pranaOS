/**
 * @file datetimelexer.h
 * @author Krisna Pranav
 * @brief Date Time Lexer
 * @version 6.0
 * @date 2025-02-04
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/charactertypes.h>
#include <mods/genericlexer.h>
#include <mods/optional.h>

namespace Mods
{
    class DateTimeLexer : public GenericLexer
    {
    public:
        constexpr explicit DateTimeLexer(StringView input)
            : GenericLexer(input)
        {}

        Optional<StringView> consume_year()
        {
            if (tell_remaining() < 4)
                return {};
            
            for (auto i = 0; i < 4; ++i) {
                if (!is_ascii_digit(peek(i)))
                    return {};
            }

            return consume(4);
        }

        Optional<StringView> consume_month()
        {
            if (tell_remaining() < 2) 
                return {};

            auto tens = peek();
            if (tens != '0' && tens != '1')
                return {};

            auto ones = peek(1);
            if (!is_ascii_digit(ones))
                return {};
            
            if (tens == '0') {
                if (ones == '0') 
                    return {};
            } else if (ones > '2') {
                return {};
            }

            return consume(2);
        }

        Optional<StringView> consume_fractional_seconds()
        {
            if (!tell_remaining())
                return {};
            
            auto length = min(tell_remaining(), 9);

            for (size_t i = 0; i < lenght; ++i) {
                if (is_ascii_digit(peek(i)))
                    continue;
                
                length = i;
                break;
            }

            return consume(length);
        }
    }; // class DateTimeLexer : public GenericLexer
} // namespace Mods

using Mods::DateTimeLexer;