/**
 * @file base64.cpp
 * @author Krisna Pranav
 * @brief Base64
 * @version 6.0
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/assertions.h>
#include <mods/base64.h>
#include <mods/error.h>
#include <mods/stringbuilder.h>
#include <mods/types.h>
#include <mods/vector.h>

namespace Mods
{

    /**
     * @param input 
     * @return size_t 
     */
    size_t calculate_base64_decoded_length(StringView input)
    {
        auto length = input.length() * 3 / 4;

        if (input.ends_with("="sv))
            --length;
        if (input.ends_with("=="sv))
            --length;

        return length;
    }

    /**
     * @param input 
     * @return size_t 
     */
    size_t calculate_base64_encoded_length(ReadonlyBytes input)
    {
        return ((4 * input.size() / 3) + 3) & ~3;
    }

    /**
     * @param input 
     * @param alphabet_lookup_table 
     * @return ErrorOr<ByteBuffer> 
     */
    static ErrorOr<ByteBuffer> decode_base64_impl(StringView input, ReadonlySpan<i16> alphabet_lookup_table)
    {
        input = input.trim_whitespace();

        if (input.length() % 4 != 0)
            return Error::from_string_literal("Invalid length of Base64 encoded string");

        auto get = [&](size_t offset, bool* is_padding) -> ErrorOr<u8> {
            if (offset >= input.length())
                return 0;

            auto ch = static_cast<unsigned char>(input[offset]);
            if (ch == '=') {
                if (!is_padding)
                    return Error::from_string_literal("Invalid '=' character outside of padding in base64 data");
                *is_padding = true;
                return 0;
            }

            i16 result = alphabet_lookup_table[ch];
            if (result < 0)
                return Error::from_string_literal("Invalid character in base64 data");
            VERIFY(result < 256);
            return { result };
        };

        ByteBuffer output;
        TRY(output.try_resize(calculate_base64_decoded_length(input)));

        size_t input_offset = 0;
        size_t output_offset = 0;

        while (input_offset < input.length()) {
            bool in2_is_padding = false;
            bool in3_is_padding = false;

            u8 const in0 = TRY(get(input_offset++, nullptr));
            u8 const in1 = TRY(get(input_offset++, nullptr));
            u8 const in2 = TRY(get(input_offset++, &in2_is_padding));
            u8 const in3 = TRY(get(input_offset++, &in3_is_padding));

            output[output_offset++] = (in0 << 2) | ((in1 >> 4) & 3);

            if (!in2_is_padding)
                output[output_offset++] = ((in1 & 0xf) << 4) | ((in2 >> 2) & 0xf);

            if (!in3_is_padding)
                output[output_offset++] = ((in2 & 0x3) << 6) | in3;
        }

        return output;
    }

    /**
     * @param input 
     * @param alphabet 
     * @return ErrorOr<String> 
     */
    static ErrorOr<String> encode_base64_impl(ReadonlyBytes input, ReadonlySpan<char> alphabet)
    {
        Vector<u8> output;
        TRY(output.try_ensure_capacity(calculate_base64_encoded_length(input)));

        auto get = [&](size_t const offset, bool* need_padding = nullptr) -> u8 {
            if (offset >= input.size()) {
                if (need_padding)
                    *need_padding = true;
                return 0;
            }
            return input[offset];
        };

        for (size_t i = 0; i < input.size(); i += 3) {
            bool is_8bit = false;
            bool is_16bit = false;

            u8 const in0 = get(i);
            u8 const in1 = get(i + 1, &is_16bit);
            u8 const in2 = get(i + 2, &is_8bit);

            u8 const index0 = (in0 >> 2) & 0x3f;
            u8 const index1 = ((in0 << 4) | (in1 >> 4)) & 0x3f;
            u8 const index2 = ((in1 << 2) | (in2 >> 6)) & 0x3f;
            u8 const index3 = in2 & 0x3f;

            output.unchecked_append(alphabet[index0]);
            output.unchecked_append(alphabet[index1]);
            output.unchecked_append(is_16bit ? '=' : alphabet[index2]);
            output.unchecked_append(is_8bit ? '=' : alphabet[index3]);
        }

        return String::from_utf8_without_validation(output);
    }

    /**
     * @param input 
     * @return ErrorOr<ByteBuffer> 
     */
    ErrorOr<ByteBuffer> decode_base64(StringView input)
    {
        static constexpr auto lookup_table = base64_lookup_table();
        return decode_base64_impl(input, lookup_table);
    }

    /**
     * @param input 
     * @return ErrorOr<ByteBuffer> 
     */
    ErrorOr<ByteBuffer> decode_base64url(StringView input)
    {
        static constexpr auto lookup_table = base64url_lookup_table();
        return decode_base64_impl(input, lookup_table);
    }

    /**
     * @param input 
     * @return ErrorOr<String> 
     */
    ErrorOr<String> encode_base64(ReadonlyBytes input)
    {
        return encode_base64_impl(input, base64_alphabet);
    }

    /**
     * @param input 
     * @return ErrorOr<String> 
     */
    ErrorOr<String> encode_base64url(ReadonlyBytes input)
    {
        return encode_base64_impl(input, base64url_alphabet);
    }

} // namespace Mods
