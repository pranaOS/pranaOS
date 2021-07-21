/*
 * Copyright (c) 2021, Krisna Pranav, evilbat831
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#include <base/CharacterTypes.h>
#include <base/Format.h>
#include <base/GenericLexer.h>
#include <base/String.h>
#include <base/StringBuilder.h>
#include <base/kstdio.h>

#if defined(__pranaos__) && !defined(KERNEL)
#    include <pranaos.h>
#endif

#ifdef KERNEL
#    include <kernel/Process.h>
#    include <kernel/Thread.h>
#else
#    include <stdio.h>
#    include <string.h>
#endif

namespace Base {

namespace {

static constexpr size_t use_next_index = NumericLimits<size_t>::max();

static constexpr size_t convert_unsigned_to_string(u64 value, Array<u8, 128>& buffer, u8 base, bool upper_case)
{
    VERIFY(base >= 2 && base <= 16);

    constexpr const char* lowercase_lookup = "0123456789abcdef";
    constexpr const char* uppercase_lookup = "0123456789ABCDEF";

    if (value == 0) {
        buffer[0] = '0';
        return 1;
    }

    size_t used = 0;
    while (value > 0) {
        if (upper_case)
            buffer[used++] = uppercase_lookup[value % base];
        else
            buffer[used++] = lowercase_lookup[value % base];

        value /= base;
    }

    for (size_t i = 0; i < used / 2; ++i)
        swap(buffer[i], buffer[used - i - 1]);

    return used;
}

void vformat_impl(TypeErasedFormatParams& params, FormatBuilder& builder, FormatParser& parser)
{
    const auto literal = parser.consume_literal();
    builder.put_literal(literal);

    FormatParser::FormatSpecifier specifier;
    if (!parser.consume_specifier(specifier)) {
        VERIFY(parser.is_eof());
        return;
    }

    if (specifier.index == use_next_index)
        specifier.index = params.take_next_index();

    auto& parameter = params.parameters().at(specifier.index);

    FormatParser argparser { specifier.flags };
    parameter.formatter(params, builder, argparser, parameter.value);

    vformat_impl(params, builder, parser);
}

} 

FormatParser::FormatParser(StringView input)
    : GenericLexer(input)
{
}
StringView FormatParser::consume_literal()
{
    const auto begin = tell();

    while (!is_eof()) {
        if (consume_specific("{{"))
            continue;

        if (consume_specific("}}"))
            continue;

        if (next_is(is_any_of("{}")))
            return m_input.substring_view(begin, tell() - begin);

        consume();
    }

    return m_input.substring_view(begin);
}
    
bool FormatParser::consume_number(size_t& value)
{
    value = 0;

    bool consumed_at_least_one = false;
    while (next_is(is_ascii_digit)) {
        value *= 10;
        value += parse_ascii_digit(consume());
        consumed_at_least_one = true;
    }

    return consumed_at_least_one;
}
bool FormatParser::consume_specifier(FormatSpecifier& specifier)
{
    VERIFY(!next_is('}'));

    if (!consume_specific('{'))
        return false;

    if (!consume_number(specifier.index))
        specifier.index = use_next_index;

    if (consume_specific(':')) {
        const auto begin = tell();

        size_t level = 1;
        while (level > 0) {
            VERIFY(!is_eof());

            if (consume_specific('{')) {
                ++level;
                continue;
            }

            if (consume_specific('}')) {
                --level;
                continue;
            }

            consume();
        }

        specifier.flags = m_input.substring_view(begin, tell() - begin - 1);
    } else {
        if (!consume_specific('}'))
            VERIFY_NOT_REACHED();

        specifier.flags = "";
    }

    return true;
}
bool FormatParser::consume_replacement_field(size_t& index)
{
    if (!consume_specific('{'))
        return false;

    if (!consume_number(index))
        index = use_next_index;

    if (!consume_specific('}'))
        VERIFY_NOT_REACHED();

    return true;
}

void FormatBuilder::put_padding(char fill, size_t amount)
{
    for (size_t i = 0; i < amount; ++i)
        m_builder.append(fill);
}
void FormatBuilder::put_literal(StringView value)
{
    for (size_t i = 0; i < value.length(); ++i) {
        m_builder.append(value[i]);
        if (value[i] == '{' || value[i] == '}')
            ++i;
    }
}
void FormatBuilder::put_string(
    StringView value,
    Align align,
    size_t min_width,
    size_t max_width,
    char fill)
{
    const auto used_by_string = min(max_width, value.length());
    const auto used_by_padding = max(min_width, used_by_string) - used_by_string;

    if (used_by_string < value.length())
        value = value.substring_view(0, used_by_string);

    if (align == Align::Left || align == Align::Default) {
        m_builder.append(value);
        put_padding(fill, used_by_padding);
    } else if (align == Align::Center) {
        const auto used_by_left_padding = used_by_padding / 2;
        const auto used_by_right_padding = ceil_div<size_t, size_t>(used_by_padding, 2);

        put_padding(fill, used_by_left_padding);
        m_builder.append(value);
        put_padding(fill, used_by_right_padding);
    } else if (align == Align::Right) {
        put_padding(fill, used_by_padding);
        m_builder.append(value);
    }
}
void FormatBuilder::put_u64(
    u64 value,
    u8 base,
    bool prefix,
    bool upper_case,
    bool zero_pad,
    Align align,
    size_t min_width,
    char fill,
    SignMode sign_mode,
    bool is_negative)
{
    if (align == Align::Default)
        align = Align::Right;

    Array<u8, 128> buffer;

    const auto used_by_digits = convert_unsigned_to_string(value, buffer, base, upper_case);

    size_t used_by_prefix = 0;
    if (align == Align::Right && zero_pad) {
        // We want String::formatted("{:#08x}", 32) to produce '0x00000020' instead of '0x000020'. This
        // behaviour differs from both fmtlib and printf, but is more intuitive.
        used_by_prefix = 0;
    } else {
        if (is_negative || sign_mode != SignMode::OnlyIfNeeded)
            used_by_prefix += 1;

        if (prefix) {
            if (base == 8)
                used_by_prefix += 1;
            else if (base == 16)
                used_by_prefix += 2;
            else if (base == 2)
                used_by_prefix += 2;
        }
    }

    const auto used_by_field = used_by_prefix + used_by_digits;
    const auto used_by_padding = max(used_by_field, min_width) - used_by_field;

    const auto put_prefix = [&]() {
        if (is_negative)
            m_builder.append('-');
        else if (sign_mode == SignMode::Always)
            m_builder.append('+');
        else if (sign_mode == SignMode::Reserved)
            m_builder.append(' ');

        if (prefix) {
            if (base == 2) {
                if (upper_case)
                    m_builder.append("0B");
                else
                    m_builder.append("0b");
            } else if (base == 8) {
                m_builder.append("0");
            } else if (base == 16) {
                if (upper_case)
                    m_builder.append("0X");
                else
                    m_builder.append("0x");
            }
        }
    };
    const auto put_digits = [&]() {
        for (size_t i = 0; i < used_by_digits; ++i)
            m_builder.append(buffer[i]);
    };

    if (align == Align::Left) {
        const auto used_by_right_padding = used_by_padding;

        put_prefix();
        put_digits();
        put_padding(fill, used_by_right_padding);
    } else if (align == Align::Center) {
        const auto used_by_left_padding = used_by_padding / 2;
        const auto used_by_right_padding = ceil_div<size_t, size_t>(used_by_padding, 2);

        put_padding(fill, used_by_left_padding);
        put_prefix();
        put_digits();
        put_padding(fill, used_by_right_padding);
    } else if (align == Align::Right) {
        const auto used_by_left_padding = used_by_padding;

        if (zero_pad) {
            put_prefix();
            put_padding('0', used_by_left_padding);
            put_digits();
        } else {
            put_padding(fill, used_by_left_padding);
            put_prefix();
            put_digits();
        }
    }
}
void FormatBuilder::put_i64(
    i64 value,
    u8 base,
    bool prefix,
    bool upper_case,
    bool zero_pad,
    Align align,
    size_t min_width,
    char fill,
    SignMode sign_mode)
{
    const auto is_negative = value < 0;
    value = is_negative ? -value : value;

    put_u64(static_cast<u64>(value), base, prefix, upper_case, zero_pad, align, min_width, fill, sign_mode, is_negative);
}

}

}
