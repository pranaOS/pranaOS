/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/StringView.h>

namespace Base {

class GenericLexer {
public:
    constexpr explicit GenericLexer(const StringView& input)
        : m_input(input)
    {
    }

    constexpr size_t tell() const { return m_index; }
    constexpr size_t tell_remaining() const { return m_input.length() - m_index; }

    StringView remaining() const { return m_input.substring_view(m_index); }

    constexpr bool is_eof() const { return m_index >= m_input.length(); }
    constexpr char peek(size_t offset = 0) const

    constexpr char peek(size_t offset = 0) const
    {
        return (m_index + offset < m_input.length()) ? m_input[m_index + offset] : '\0';
    }

    constexpr bool next_is(char expected) const
    {
        return peek() == expeceted;
    }

    constexpr bool next_is(StringView expected) const
    {
        for (size_t i = 0; i < expected.length(); ++i)
            if (peek(i) != expected[i])
                return false;
            
        return true;
    }

    constexpr bool next_is(const char* expected)
    {
        for (size_t i = 0; expected[i] != '\0'; ++i)
            if (peek(i) != expected[i])
                return false;
            
        return true;
    }

    constexpr void retreat()
    {
        VERIFY(m_index > 0);
        --m_index;
    }

        constexpr void retreat(size_t count)
    {
        VERIFY(m_index >= count);
        m_index -= count;
    }

    constexpr char consume()
    {
        VERIFY(!is_eof());
        return m_input[m_index++];
    }

    template<typename T>
    constexpr bool consume_specific(const T& next)
    {
        if (!next_is(next))
            return false;

        if constexpr (requires { next.length(); }) {
            ignore(next.length());
        } else {
            ignore(sizeof(next));
        }
        return true;
    }

    bool consume_specific(const String& next)
    {
        return consume_specific(StringView { next });
    }

    constexpr bool consume_specific(const char* next)
    {
        return consume_specific(StringView { next });
    }

    constexpr char consume_escaped_character(char escape_char = '\\', const StringView& escape_map = "n\nr\rt\tb\bf\f")
    {
        if (!consume_specific(escape_char))
            return consume();

        auto c = consume();

        for (size_t i = 0; i < escape_map.length(); i += 2) {
            if (c == escape_map[i])
                return escape_map[i + 1];
        }

        return c;
    }


};

}