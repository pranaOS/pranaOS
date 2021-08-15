/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include "Selector.h"
#include <base/StringUtils.h>
#include <ctype.h>

namespace Web::CSS {

Selector::Selector(Vector<CompoundSelector>&& compound_selectors)
    : m_compound_selectors(move(compound_selectors))
{
}

Selector::~Selector()
{
}

u32 Selector::specificity() const
{
    unsigned ids = 0;
    unsigned tag_names = 0;
    unsigned classes = 0;

    for (auto& list : m_compound_selectors) {
        for (auto& simple_selector : list.simple_selectors) {
            switch (simple_selector.type) {
            case SimpleSelector::Type::Id:
                ++ids;
                break;
            case SimpleSelector::Type::Class:
                ++classes;
                break;
            case SimpleSelector::Type::TagName:
                ++tag_names;
                break;
            default:
                break;
            }
        }
    }

    return ids * 0x10000 + classes * 0x100 + tag_names;
}

Selector::SimpleSelector::ANPlusBPattern Selector::SimpleSelector::ANPlusBPattern::parse(StringView const& args)
{
    CSS::Selector::SimpleSelector::ANPlusBPattern pattern;
    if (args.equals_ignoring_case("odd")) {
        pattern.step_size = 2;
        pattern.offset = 1;
    } else if (args.equals_ignoring_case("even")) {
        pattern.step_size = 2;
    } else {
        auto const consume_int = [](GenericLexer& lexer) -> Optional<int> {
            return Base::StringUtils::convert_to_int(lexer.consume_while([](char c) -> bool {
                return isdigit(c) || c == '+' || c == '-';
            }));
        };

        int step_size_or_offset = 0;
        GenericLexer lexer { args };

        if (lexer.consume_specific("n") || lexer.consume_specific("+n")) {
            step_size_or_offset = +1;
            lexer.retreat();
        } else if (lexer.consume_specific("-n")) {
            step_size_or_offset = -1;
            lexer.retreat();
        } else {
            auto const value = consume_int(lexer);
            if (!value.has_value())
                return {};
            step_size_or_offset = value.value();
        }

        if (lexer.consume_specific("n")) {
            lexer.ignore_while(isspace);
            if (lexer.next_is('+') || lexer.next_is('-')) {
                auto const sign = lexer.next_is('+') ? 1 : -1;
                lexer.ignore();
                lexer.ignore_while(isspace);

                auto const offset = consume_int(lexer);
                if (!offset.has_value())
                    return {};
                pattern.step_size = step_size_or_offset;
                pattern.offset = sign * offset.value();
            } else {

                pattern.step_size = step_size_or_offset;
            }
        } else {

            pattern.offset = step_size_or_offset;
        }

        if (lexer.remaining().length() > 0)
            return {};
    }

    return pattern;
}

}
