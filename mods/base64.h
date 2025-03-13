/**
 * @file base64.h
 * @author Krisna Pranav
 * @brief Base64
 * @version 6.0
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/array.h>
#include <mods/bytebuffer.h>
#include <mods/error.h>
#include <mods/string.h>
#include <mods/stringview.h>

namespace Mods 
{

    constexpr Array base64_alphabet = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
        'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
        'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
        'w', 'x', 'y', 'z', '0', '1', '2', '3',
        '4', '5', '6', '7', '8', '9', '+', '/'
    };

    constexpr Array base64url_alphabet = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
        'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
        'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
        'w', 'x', 'y', 'z', '0', '1', '2', '3',
        '4', '5', '6', '7', '8', '9', '-', '_'
    };

    /**
     * @return consteval 
     */
    consteval auto base64_lookup_table()
    {
        Array<i16, 256> table;
        table.fill(-1);
        for (size_t i = 0; i < base64_alphabet.size(); ++i) {
            table[base64_alphabet[i]] = static_cast<i16>(i);
        }
        return table;
    }

    /**
     * @return consteval 
     */
    consteval auto base64url_lookup_table()
    {
        Array<i16, 256> table;
        table.fill(-1);
        for (size_t i = 0; i < base64url_alphabet.size(); ++i) {
            table[base64url_alphabet[i]] = static_cast<i16>(i);
        }
        return table;
    }

    /**
     * @return size_t 
     */
    [[nodiscard]] size_t calculate_base64_decoded_length(StringView);

    [[nodiscard]] size_t calculate_base64_encoded_length(ReadonlyBytes);

    /**
     * @return ErrorOr<ByteBuffer> 
     */
    [[nodiscard]] ErrorOr<ByteBuffer> decode_base64(StringView);
    [[nodiscard]] ErrorOr<ByteBuffer> decode_base64url(StringView);

    [[nodiscard]] ErrorOr<String> encode_base64(ReadonlyBytes);
    [[nodiscard]] ErrorOr<String> encode_base64url(ReadonlyBytes);

} // namespace Mods


#if USING_MODS_GLOBALLY
using Mods::decode_base64;
using Mods::decode_base64url;
using Mods::encode_base64;
using Mods::encode_base64url;
#endif
