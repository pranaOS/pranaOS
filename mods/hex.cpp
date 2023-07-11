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

#include "array.h"
#include "byte_buffer.h"
#include "hex.h"
#include "string.h"
#include "string_builder.h"
#include "string_view.h"
#include "types.h"
#include "vector.h"
#include "platform.h"

namespace AK {

    /**
     * @param digit 
     * @return u8 
     */
    static u8 decode_hex_digit(char digit)
    {
        if (digit >= '0' && digit <= '9')
            return digit - '0';
        if (digit >= 'a' && digit <= 'f')
            return 10 + (digit - 'a');
        if (digit >= 'A' && digit <= 'F')
            return 10 + (digit - 'A');
        return 255;
    }

    /**
     * @param input 
     * @return Optional<ByteBuffer> 
     */
    Optional<ByteBuffer> decode_hex(const StringView& input)
    {
        if ((input.length() % 2) != 0)
            return {};

        auto output = ByteBuffer::create_zeroed(input.length() / 2);

        for (long unsigned int i = 0; i < input.length() / 2; i++) {
            auto c1 = decode_hex_digit(input[i * 2]);
            if (c1 >= 16)
                return {};

            auto c2 = decode_hex_digit(input[i * 2 + 1]);
            if (c2 >= 16)
                return {};

            output[i] = (c1 << 4) + c2;
        }

        return output;
    }

    /**
     * @param input 
     * @return String 
     */
    String encode_hex(ReadonlyBytes input)
    {
        StringBuilder output(input.size() * 2);

        for (auto ch : input)
            output.appendf("%02x", ch);

        return output.build();
    }

}