/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

/* includes */
#include <libutils/bytebuffer.h>
#include <libutils/optional.h>
#include <libutils/stringutils.h>
#include <string>

namespace Utils {

constexpr u8 decode_hex_digit(char digit)
{
    if (digit >= '0' && digit <= '9')
        return digit - '0';
    if (digit >= 'a' && digit <= 'f')
        return 10 + (digit - 'a');
    if (digit >= 'A' && digit <= 'F')
        return 10 + (digit - 'A');
    return 255;
}

Optional<ByteBuffer> decode_hex(const StringView&);

String encode_hex(ReadonlyBytes);

}

using Utils::decode_hex;
using Utils::decode_hex_digit;
using Utils::encode_hex;