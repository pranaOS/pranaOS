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

};

}