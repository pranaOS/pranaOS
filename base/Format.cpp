/*
 * Copyright (c) 2021, Krisna Pranav
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

}

}