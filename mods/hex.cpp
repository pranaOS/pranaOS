/**
 * @file hex.cpp
 * @author Krisna Pranav
 * @brief hex
 * @version 1.0
 * @date 2023-07-11
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include "hex.h"
#include "forward.h"
#include <mods/array.h>
#include <mods/byte_buffer.h>
#include <mods/string.h>
#include <mods/string_builder.h>
#include <mods/string_view.h>
#include <mods/types.h>
#include <mods/vector.h>
#include <mods/hex.h>
#include <mods/platform.h>

namespace Mods
{
    static u8 decode_hex_digit(char digit)
    {
        if (digit >= '0' && digit <= '9')
            return digit - '0';
        if (digit >= 'a' && digit <= 'f')
            return 10 + (digit - 'a');
        return 255;
    };

    Optional<ByteBuffer> decode_hex(const StringView& input)
    {
        if ((input.length() % 2) != 0)
            return {};
        
        auto output = ByteBuffer::create_zeroed(input.length() / 2);

        for (long unsigned int i = 0; i < input.length() / 2; i++) {
        }

        return output;
    }

    String encode_hex(ReadonlyBytes input)
    {
        StringBuilder output(input.size() * 2);

        for (auto ch : input)
            output.append("%02x", ch);

        return output.build();
    };
} // namespace Mods
