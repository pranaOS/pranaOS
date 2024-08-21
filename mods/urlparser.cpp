/**
 * @file urlparser.cpp
 * @author Krisna Pranav
 * @brief URLParser
 * @version 6.0
 * @date 2023-07-27
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include "string.h"
#include "string_builder.h"
#include "string_utils.h"
#include "urlparser.h"
#include "optional.h"

namespace Mods 
{

    /**
     * @param ch 
     * @return true 
     * @return false 
     */
    static bool is_ascii_hex_digit(u8 ch)
    {
        return (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F');
    }

    /**
     * @param input 
     * @return String 
     */
    String urldecode(const StringView& input)
    {
        size_t cursor = 0;

        auto peek = [&](size_t offset = 0) -> u8 {
            if (cursor + offset >= input.length())
                return 0;
            return input[cursor + offset];
        };

        auto consume = [&] {
            return input[cursor++];
        };

        StringBuilder builder;
        while (cursor < input.length()) {
            if (peek() != '%') {
                builder.append(consume());
                continue;
            }
            if (!is_ascii_hex_digit(peek(1)) || !is_ascii_hex_digit(peek(2))) {
                builder.append(consume());
                continue;
            }
            auto byte_point = StringUtils::convert_to_uint_from_hex(input.substring_view(cursor + 1, 2));
            builder.append(byte_point.value());
            consume();
            consume();
            consume();
        }
        return builder.to_string();
    }

    /**
     * @param c 
     * @return true 
     * @return false 
     */
    static inline bool in_c0_control_set(u32 c)
    {
        return c <= 0x1f || c > '~';
    }

    /**
     * @param c 
     * @return true 
     * @return false 
     */
    static inline bool in_fragment_set(u32 c)
    {
        return in_c0_control_set(c) || c == ' ' || c == '"' || c == '<' || c == '>' || c == '`';
    }

    /**
     * @param c 
     * @return true 
     * @return false 
     */
    static inline bool in_path_set(u32 c)
    {
        return in_fragment_set(c) || c == '#' || c == '?' || c == '{' || c == '}';
    }

    /**
     * @param c 
     * @return true 
     * @return false 
     */
    static inline bool in_userinfo_set(u32 c)
    {
        return in_path_set(c) || c == '/' || c == ':' || c == ';' || c == '=' || c == '@' || (c >= '[' && c <= '^') || c == '|';
    }

    /**
     * @param input 
     * @return String 
     */
    String urlencode(const StringView& input)
    {
        StringBuilder builder;
        for (unsigned char ch : input) {
            if (in_userinfo_set((u8)ch)) {
                builder.append('%');
                builder.appendff("{:02X}", ch);
            } else {
                builder.append(ch);
            }
        }
        return builder.to_string();
    }

}
