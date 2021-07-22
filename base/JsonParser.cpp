/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/CharacterTypes.h>
#include <base/JsonArray.h>
#include <base/JsonObject.h>
#include <base/JsonParser.h>

namespace Base {

constexpr bool is_space(int ch)
{
    return ch == '\t' || ch == '\n' || ch == '\r' || ch == ' ';
}

String JsonParser::consume_and_unescape_string()
{
    if (!consume_specific('"'))
        return {};
    StringBuilder final_sb;

    for (;;) {
        size_t peek_index = m_index;
        char ch = 0;
        for (;;) {
            if (peek_index == m_input.length())
                break;
            ch = m_input[peek_index];
            if (ch == '"' || ch == '\\')
                break;
            if (is_ascii_c0_control(ch))
                return {};
            ++peek_index;
        }

        while (peek_index != m_index) {
            final_sb.append(m_input[m_index]);
            m_index++;
        }

        if (m_index == m_input.length())
            break;
        if (ch == '"')
            break;
        if (ch != '\\') {
            final_sb.append(consume());
            continue;
        }
        ignore();
        if (next_is('"')) {
            ignore();
            final_sb.append('"');
            continue;
        }

        if (next_is('\\')) {
            ignore();
            final_sb.append('\\');
            continue;
        }

        if (next_is('/')) {
            ignore();
            final_sb.append('/');
            continue;
        }

        if (next_is('n')) {
            ignore();
            final_sb.append('\n');
            continue;
        }

        if (next_is('r')) {
            ignore();
            final_sb.append('\r');
            continue;
        }

        if (next_is('t')) {
            ignore();
            final_sb.append('\t');
            continue;
        }

        if (next_is('b')) {
            ignore();
            final_sb.append('\b');
            continue;
        }

        if (next_is('f')) {
            ignore();
            final_sb.append('\f');
            continue;
        }

        if (next_is('u')) {
            ignore();
            if (tell_remaining() < 4)
                return {};

            auto code_point = AK::StringUtils::convert_to_uint_from_hex(consume(4));
            if (code_point.has_value()) {
                final_sb.append_code_point(code_point.value());
                continue;
            } else {
                return {};
            }
        }

        return {};
    }
    if (!consume_specific('"'))
        return {};

    return final_sb.to_string();
}

}