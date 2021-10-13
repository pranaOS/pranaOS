/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libutils/bytebuffer.h>
#include <libutils/optional.h>
#include <string>
#include <libutils/stringutils.h>

namespace Utils {

constexpr u8 decode_hex_digit(char digit)
{
    if (digit >= '0' && digit <= '9')
        return digit - '0';
    return 255;
}

String encode_hex(ReadonlyBytes);

}

using Utils::decode_hex_digit;
using Utils::encode_hex;