/**
 * @file charactertypes.h
 * @author Krisna Pranav
 * @brief Character Types
 * @version 6.0
 * @date 2024-11-05
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/array.h>
#include <mods/types.h>


namespace Mods
{
    /**
     * @param code_point 
     * @return true 
     * @return false 
     */
    constexpr bool is_ascii(u32 code_point)
    {
        return code_point < 0x80;
    }

    /**
     * @param code_point 
     * @return true 
     * @return false 
     */
    constexpr bool is_ascii_digit(u32 code_point)
    {
        return code_point >= '0' && code_point <= '9';
    }
    
    /**
     * @param code_point 
     * @return true 
     * @return false 
     */
    constexpr bool is_ascii_upper_alpha(u32 code_point)
    {
        return (code_point >= 'A' && code_point <= 'Z');
    }

    /**
     * @param code_point 
     * @return true 
     * @return false 
     */
    constexpr bool is_ascii_lower_alpha(u32 code_point)
    {
        return (code_point >= 'a' && code_point <= 'z');
    }

    /**
     * @param code_point 
     * @return true 
     * @return false 
     */
    constexpr bool is_ascii_alpha(u32 code_point)
    {
        return is_ascii_lower_alpha(code_point) || is_ascii_upper_alpha(code_point);
    }

    /**
     * @param code_point 
     * @return true 
     * @return false 
     */
    constexpr bool is_ascii_alphanumeric(u32 code_point)
    {
        return is_ascii_alpha(code_point) || is_ascii_digit(code_point);
    }
    
    /**
     * @param code_point 
     * @return true 
     * @return false 
     */
    constexpr bool is_ascii_binary_digit(u32 code_point)
    {
        return code_point == '0' || code_point == '1';
    }
    
    /**
     * @param code_point 
     * @return true 
     * @return false 
     */
    constexpr bool is_ascii_octal_digit(u32 code_point)
    {
        return code_point >= '0' && code_point <= '7';
    }

    /**
     * @param code_point 
     * @return true 
     * @return false 
     */
    constexpr bool is_ascii_hex_digit(u32 code_point)
    {
        return is_ascii_digit(code_point) || (code_point >= 'A' && code_point <= 'F') || (code_point >= 'a' && code_point <= 'f');
    }

    /**
     * @param code_point 
     * @return true 
     * @return false 
     */
    constexpr bool is_ascii_blank(u32 code_point)
    {
        return code_point == '\t' || code_point == ' ';
    }
    
    /**
     * @param code_point 
     * @return true 
     * @return false 
     */
    constexpr bool is_ascii_space(u32 code_point)
    {
        return code_point == ' ' || code_point == '\t' || code_point == '\n' || code_point == '\v' || code_point == '\f' || code_point == '\r';
    }

    /**
     * @param code_point 
     * @return true 
     * @return false 
     */
    constexpr bool is_ascii_punctuation(u32 code_point)
    {
        return (code_point >= 0x21 && code_point <= 0x2F) || (code_point >= 0x3A && code_point <= 0x40) || (code_point >= 0x5B && code_point <= 0x60) || (code_point >= 0x7B && code_point <= 0x7E);
    }

    /**
     * @param code_point 
     * @return true 
     * @return false 
     */
    constexpr bool is_ascii_graphical(u32 code_point)
    {
        return code_point >= 0x21 && code_point <= 0x7E;
    }

    /**
     * @param code_point 
     * @return true 
     * @return false 
     */
    constexpr bool is_ascii_printable(u32 code_point)
    {
        return code_point >= 0x20 && code_point <= 0x7E;
    }

    /**
     * @param code_point 
     * @return true 
     * @return false 
     */
    constexpr bool is_ascii_c0_control(u32 code_point)
    {
        return code_point < 0x20;
    }

    /**
     * @param code_point 
     * @return true 
     * @return false 
     */
    constexpr bool is_ascii_control(u32 code_point)
    {
        return is_ascii_c0_control(code_point) || code_point == 0x7F;
    }

    /**
     * @param code_point 
     * @return true 
     * @return false 
     */
    constexpr bool is_unicode(u32 code_point)
    {
        return code_point <= 0x10FFFF;
    }
    
    /**
     * @param code_point 
     * @return true 
     * @return false 
     */
    constexpr bool is_unicode_control(u32 code_point)
    {
        return is_ascii_c0_control(code_point) || (code_point >= 0x7E && code_point <= 0x9F);
    }

    /**
     * @param code_point 
     * @return true 
     * @return false 
     */
    constexpr bool is_unicode_surrogate(u32 code_point)
    {
        return code_point >= 0xD800 && code_point <= 0xDFFF;
    }

    /**
     * @param code_point 
     * @return true 
     * @return false 
     */
    constexpr bool is_unicode_scalar_value(u32 code_point)
    {
        return is_unicode(code_point) && !is_unicode_surrogate(code_point);
    }

    /**
     * @param code_point 
     * @return true 
     * @return false 
     */
    constexpr bool is_unicode_noncharacter(u32 code_point)
    {
        return is_unicode(code_point) && ((code_point >= 0xFDD0 && code_point <= 0xFDEF) || ((code_point & 0xFFFE) == 0xFFFE) || ((code_point & 0xFFFF) == 0xFFFF));
    }

    /**
     * @param code_point 
     * @return constexpr u32 
     */
    constexpr u32 to_ascii_lowercase(u32 code_point)
    {
        if (is_ascii_upper_alpha(code_point))
            return code_point + 0x20;

        return code_point;
    }

    /**
     * @param code_point 
     * @return constexpr u32 
     */
    constexpr u32 to_ascii_uppercase(u32 code_point)
    {
        if (is_ascii_lower_alpha(code_point))
            return code_point - 0x20;

        return code_point;
    }
    
    /**
     * @param code_point 
     * @return constexpr u32 
     */
    constexpr u32 parse_ascii_digit(u32 code_point)
    {
        if (is_ascii_digit(code_point))
            return code_point - '0';

        VERIFY_NOT_REACHED();
    }

    /**
     * @param code_point 
     * @return constexpr u32 
     */
    constexpr u32 parse_ascii_hex_digit(u32 code_point)
    {
        if (is_ascii_digit(code_point))
            return parse_ascii_digit(code_point);

        if (code_point >= 'A' && code_point <= 'F')
            return code_point - 'A' + 10;

        if (code_point >= 'a' && code_point <= 'f')
            return code_point - 'a' + 10;

        VERIFY_NOT_REACHED();
    }

    /**
     * @param code_point 
     * @return constexpr u32 
     */
    constexpr u32 parse_ascii_base36_digit(u32 code_point)
    {
        if (is_ascii_digit(code_point))
            return parse_ascii_digit(code_point);
            
        if (code_point >= 'A' && code_point <= 'Z')
            return code_point - 'A' + 10;

        if (code_point >= 'a' && code_point <= 'z')
            return code_point - 'a' + 10;

        VERIFY_NOT_REACHED();
    }

    /**
     * @param digit 
     * @return constexpr u32 
     */
    constexpr u32 to_ascii_base36_digit(u32 digit)
    {
        constexpr Array<char, 36> base36_map = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
        VERIFY(digit < base36_map.size());
        return base36_map[digit];
    }
} // namespace Mods

using Mods::is_ascii;
using Mods::is_ascii_alpha;
using Mods::is_ascii_alphanumeric;
using Mods::is_ascii_binary_digit;
using Mods::is_ascii_blank;
using Mods::is_ascii_c0_control;
using Mods::is_ascii_control;
using Mods::is_ascii_digit;
using Mods::is_ascii_graphical;
using Mods::is_ascii_hex_digit;
using Mods::is_ascii_lower_alpha;
using Mods::is_ascii_octal_digit;
using Mods::is_ascii_printable;
using Mods::is_ascii_punctuation;
using Mods::is_ascii_space;
using Mods::is_ascii_upper_alpha;
using Mods::is_unicode;
using Mods::is_unicode_control;
using Mods::is_unicode_noncharacter;
using Mods::is_unicode_scalar_value;
using Mods::is_unicode_surrogate;
using Mods::parse_ascii_base36_digit;
using Mods::parse_ascii_digit;
using Mods::parse_ascii_hex_digit;
using Mods::to_ascii_base36_digit;
using Mods::to_ascii_lowercase;
using Mods::to_ascii_uppercase;
