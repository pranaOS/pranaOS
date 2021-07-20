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

}