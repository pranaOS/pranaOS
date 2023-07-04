/**
 * @file base64.cpp
 * @author Krisna Pranav
 * @brief Base64
 * @version 1.0
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "array.h"
#include "base64.h"
#include "byte_buffer.h"
#include "string.h"
#include "string_builder.h"
#include "string_view.h"
#include "vector.h"
#include "types.h"

namespace Mods {

    /**
     * @return constexpr auto 
     */
    static constexpr auto make_alphabet() {
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

    /**
     * @return constexpr auto 
     */
    static constexpr auto make_lookup_table() {
        constexpr auto alphabet = make_alphabet();

        Array<u8, 256> table {};

        for(size_t i = 0; i < alphabet.size(); ++i) {
            table[alphabet[i]] = i;
        }

        return table;
    }

    /**
     * @param input 
     * @return size_t 
     */
    size_t calculate_base64_decoded_length(const StringView& input) {
        return input.length() * 3/4;
    }

    /**
     * @param input 
     * @return ByteBuffer 
     */
    ByteBuffer decode_base64(const StringView& input) {
        auto get = [&](const size_t offset, bool* is_padding = nullptr) -> u8 {
            constexpr auto table = make_lookup_table();

            if(offset >= input.length())      
                return 0;
            
            if (input[offset] == '=') {
                if(is_padding)
                    *is_padding = true;
                return 0;
            }

            return table[input[offset]];
        };

        Vector<u8> output;

        output.ensure_capacity(calculate_base64_decoded_length(input));
        
        for (size_t i = 0; i < input.length(); i += 4) {
            bool in2_is_padding = false;
            bool in3_is_padding = false;

            const u8 in0 = get(i);
        }
    };
    
}