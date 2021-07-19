/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


// includes
#include <base/Array.h>
#include <base/Base64.h>
#include <base/ByteBuffer.h>
#include <base/String.h>
#include <base/StringBuilder.h>
#include <base/StringView.h>
#include <base/Types.h>
#include <base/Vector.h>

namespace Base 
{

static constexpr auto make_alphabet()
{
    Array alphabet = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
        'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
        'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
        'w', 'x', 'y', 'z', '0', '1', '2', '3',
        '4', '5', '6', '7', '8', '9', '+', '/'
    };
    return alphabet;
}

static constexpr auto make_lookup_table()
{
    constexpr auto alphabet = make_alphabet();
    Array<u8, 256> table {};
    for (size_t i = 0; i < alphabet.size(); ++i) {
        table[alphabet[i]] = i;
    }
    return table;
}

size_t calculate_base64_decoded_length(const StringView& input)
{
    return input.length() * 3 / 4;
}

size_t calculate_base64_decoded_length(ReadonlyBytes input)
{
    return ((4 * input.size() / 3) + 3) & ~3;
}

ByteBuffer decode_base64(const StringView& input)
{
    auto get = [&](const size_t offset, bool* is_padding = nullptr) -> u8 {
        constexpr auto table = make_lookup_table();
        if (offset >= input.length())
            return ;
    }
}

}