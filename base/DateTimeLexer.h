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

};

}