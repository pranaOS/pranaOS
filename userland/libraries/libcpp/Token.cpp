/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include "Token.h"
#include <base/String.h>

namespace Cpp {

bool Position::operator<(const Position& other) const
{
    return line < other.line || (line == other.line && column < other.column);
}
bool Position::operator>(const Position& other) const
{
    return !(*this < other) && !(*this == other);
}
bool Position::operator==(const Position& other) const
{
    return line == other.line && column == other.column;
}
bool Position::operator<=(const Position& other) const
{
    return !(*this > other);
}

String Token::to_string() const
{
    return String::formatted("{}  {}:{}-{}:{} ({})", type_to_string(m_type), start().line, start().column, end().line, end().column, text());
}

String Token::type_as_string() const
{
    return type_to_string(m_type);
}

}
