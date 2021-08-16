/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include "libcpp/Token.h"
#include <base/StringView.h>
#include <base/Vector.h>

namespace Cpp {

class Lexer {
public:
    explicit Lexer(StringView const&, size_t start_line = 0);

    Vector<Token> lex();

private:
    char peek(size_t offset = 0) const;
    char consume();

    StringView m_input;
    size_t m_index { 0 };
    Position m_previous_position { 0, 0 };
    Position m_position { 0, 0 };
};

}
