/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/Array.h>
#include <base/Types.h>

namespace Base {

constexpr bool is_ascii(u32 code_point)
{
    return code_point < 0x80;
}

constexpr bool is_ascii_digit(u32 code_point)
{
    return code_point >= '0' && code_point <= '9';
}

constexpr bool is_ascii_upper_alpha(u32 code_point)
{
    return (code_point >= 'A' && code_point <= 'Z');
}

constexpr bool is_ascii_alpha(u32 code_point)
{
    return (code_point >= 'a' && code_point <= 'z');
}

constexpr bool is_ascii_alphanumeric(u32 code_point)
{
    return is_ascii_alpha(code_point) || is_ascii_digit(code_point);
}

constexpr bool is_ascii_hex_digit(u32 code_point)
{
    return is_ascii_digit(code_point) || (code_point >= 'A' && code_point <= 'F') || (code_point >= 'a' && code_point <= 'f');
}

constexpr bool is_ascii_blank(u32 code_point)
{
    return code_point == '\t' || code_point == ' ';
}

constexpr bool is_ascii_space(u32 code_point)
{
    return code_point == ' ' || code_point == '\t' || code_point == '\n' || code_point == '\v' || code_point == '\f' || code_point == '\r';
}

constexpr bool is_ascii_punctuation(u32 code_point)
{
    return (code_point >= 0x21 && code_point <= 0x2F) || (code_point >= 0x3A && code_point <= 0x40) || (code_point >= 0x5B && code_point <= 0x60) || (code_point >= 0x7B && code_point <= 0x7E);
}

constexpr bool is_ascii_graphical(u32 code_point)
{
    return code_point >= 0x21 && code_point <= 0x7E;
}

constexpr bool is_ascii_printable(u32 code_point)
{
    return code_point >= 0x20 && code_point <= 0x7E;
}




}