/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <base/CharacterTypes.h>
#include <base/GenericLexer.h>
#include <base/Optional.h>

namespace Base {

class DateTimeLexer : public GenericLexer {
public:
    constexpr explicit DateTimeLexer(const StringView& input)
        : GenericLexer(input)
    {
    }

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


    Optional<StringView> consume_day()
    {
        if (tell_remaining() < 2)
            return {};

        auto tens = peek();
        if (tens < '0' || tens > '3')
            return {};

        auto ones = peek(1);
        if (!is_ascii_digit(ones))
            return {};

        if (tens == '0') { 
            if (ones == '0')
                return {};
        } else if (tens == '3') { 
            if (ones != '0' && ones != '1')
                return {};
        } else if (!is_ascii_digit(ones)) { 
            return {};
        }

        return consume(2);
    }

    Optional<StringView> consume_sign()
    {
        if (!tell_remaining())
            return {};

        if (next_is("\xE2\x88\x92"sv))
            return consume(3);
        else if (next_is('-') || next_is('+'))
            return consume(1);
        else
            return {};
    };

};

}