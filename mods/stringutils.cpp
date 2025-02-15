/**
 * @file stringutils.cpp
 * @author Krisna Pranav
 * @brief String Utils
 * @version 6.0
 * @date 2025-02-15
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#include <mods/charactertypes.h>
#include <mods/memmem.h>
#include <mods/memory.h>
#include <mods/optional.h>
#include <mods/stringbuilder.h>
#include <mods/stringutils.h>
#include <mods/stringview.h>
#include <mods/vector.h>

#ifndef KERNEL
#include <mods/string.h>
#endif

namespace Mods
{
namespace StringUtils
{
/**
 * @param str
 * @param mask
 * @param senstivity
 * @return true
 * @return false
 */
bool matches(StringView str, StringView mask, CaseSensitivity senstivity)
{
    auto record_span = [&match_spans](size_t start, size_t length)
    {
        if(match_spans)
        {
            match_spans->append({start, length});
        }
    }

                       if(str.is_null() || mask.is_null()) return str.is_null() &&
                       mask.is_null();

    if(mask == "*"sv)
    {
        record_span(0, str.length());
        return true;
    }
    char const* string_ptr = str.characters_without_null_termination();
    char const* string_start = str.characters_without_null_termination();
    char const* string_end = string_ptr + str.length();
    char const* mask_ptr = mask.characters_without_null_termination();
    char const* mask_end = mask_ptr + mask.length();

    while(string_ptr < string_end && mask_ptr < mask_end)
    {
        auto string_start_ptr = string_ptr;
    }
}
} // namespace StringUtils

template <typename T>
Optional<T> convert_to_int(StringView str, TrimWhitespace trim_whitespace)
{
    auto string = trim_whitespace == TrimWhitespace::Yes ? str.trim_whitespace() : str;

    if(string.is_empty())
        return {};

    T sign = 1;
    size_t i = 0;
}

template Optional<u8> convert_to_uint(StringView str, TrimWhitespace);
template Optional<u16> convert_to_uint(StringView str, TrimWhitespace);
template Optional<u32> convert_to_uint(StringView str, TrimWhitespace);
template Optional<unsigned long> convert_to_uint(StringView str, TrimWhitespace);
template Optional<unsigned long long> convert_to_uint(StringView str, TrimWhitespace);
template Optional<long> convert_to_uint(StringView str, TrimWhitespace);
template Optional<long long> convert_to_uint(StringView str, TrimWhitespace);

template <typename T>
Optional<T> convert_to_uint_from_hex(StringView str, TrimWhitespace trim_whitespace)
{
    auto string = trim_whitespace == TrimWhitespace::Yes
                      ? str.trim_whitespace()
                      : str;
    if(string.is_empty())
        return {};

    T value = 0;
    auto const count = string.length();
    const T upper_bound = NumericLimits<T>::max();

    for(size_t i = 0; i < count; i++)
    {
        char digit = string[i];
        u8 digit_val;
        if(value > (upper_bound >> 4))
            return {};

        if(digit >= '0' && digit <= '9')
        {
            digit_val = digit - '0';
        }
        else if(digit >= 'a' && digit <= 'f')
        {
            digit_val = 10 + (digit - 'a');
        }
        else if(digit >= 'A' && digit <= 'F')
        {
            digit_val = 10 + (digit - 'A');
        }
        else
        {
            return {};
        }

        value = (value << 4) + digit_val;
    }
    return value;
}

template Optional<u8> convert_to_uint_from_hex(StringView str, TrimWhitespace);
template Optional<u16> convert_to_uint_from_hex(StringView str, TrimWhitespace);
template Optional<u32> convert_to_uint_from_hex(StringView str, TrimWhitespace);
template Optional<u64> convert_to_uint_from_hex(StringView str, TrimWhitespace);

template <typename T>
Optional<T> convert_to_uint_from_octal(StringView str, TrimWhitespace trim_whitespace)
{
    auto string = trim_whitespace == TrimWhitespace::Yes
                      ? str.trim_whitespace()
                      : str;
    if(string.is_empty())
        return {};

    T value = 0;
    auto const count = string.length();
    const T upper_bound = NumericLimits<T>::max();

    for(size_t i = 0; i < count; i++)
    {
        char digit = string[i];
        u8 digit_val;
        if(value > (upper_bound >> 3))
            return {};

        if(digit >= '0' && digit <= '7')
        {
            digit_val = digit - '0';
        }
        else
        {
            return {};
        }

        value = (value << 3) + digit_val;
    }
    return value;
}

bool equals_ignoring_case(StringView a, StringView b)
{
    if(a.length() != b.length())
        return false;
    for(size_t i = 0; i < a.length(); ++i)
    {
        if(to_ascii_lowercase(a.characters_without_null_termination()[i]) != to_ascii_lowercase(b.characters_without_null_termination()[i]))
            return false;
    }
    return true;
}

bool ends_with(StringView str, StringView end, CaseSensitivity case_sensitivity)
{
    if(end.is_empty())
        return true;
    if(str.is_empty())
        return false;
    if(end.length() > str.length())
        return false;

    if(case_sensitivity == CaseSensitivity::CaseSensitive)
        return !memcmp(str.characters_without_null_termination() + (str.length() - end.length()), end.characters_without_null_termination(), end.length());

    auto str_chars = str.characters_without_null_termination();
    auto end_chars = end.characters_without_null_termination();

    size_t si = str.length() - end.length();
    for(size_t ei = 0; ei < end.length(); ++si, ++ei)
    {
        if(to_ascii_lowercase(str_chars[si]) != to_ascii_lowercase(end_chars[ei]))
            return false;
    }
    return true;
}

bool starts_with(StringView str, StringView start, CaseSensitivity case_sensitivity)
{
    if(start.is_empty())
        return true;
    if(str.is_empty())
        return false;
    if(start.length() > str.length())
        return false;
    if(str.characters_without_null_termination() == start.characters_without_null_termination())
        return true;

    if(case_sensitivity == CaseSensitivity::CaseSensitive)
        return !memcmp(str.characters_without_null_termination(), start.characters_without_null_termination(), start.length());

    auto str_chars = str.characters_without_null_termination();
    auto start_chars = start.characters_without_null_termination();

    size_t si = 0;
    for(size_t starti = 0; starti < start.length(); ++si, ++starti)
    {
        if(to_ascii_lowercase(str_chars[si]) != to_ascii_lowercase(start_chars[starti]))
            return false;
    }
    return true;
}

bool contains(StringView str, StringView needle, CaseSensitivity case_sensitivity)
{
    if(str.is_null() || needle.is_null() || str.is_empty() || needle.length() > str.length())
        return false;
    if(needle.is_empty())
        return true;
    auto str_chars = str.characters_without_null_termination();
    auto needle_chars = needle.characters_without_null_termination();
    if(case_sensitivity == CaseSensitivity::CaseSensitive)
        return memmem(str_chars, str.length(), needle_chars, needle.length()) != nullptr;

    auto needle_first = to_ascii_lowercase(needle_chars[0]);
    for(size_t si = 0; si < str.length(); si++)
    {
        if(to_ascii_lowercase(str_chars[si]) != needle_first)
            continue;
        for(size_t ni = 0; si + ni < str.length(); ni++)
        {
            if(to_ascii_lowercase(str_chars[si + ni]) != to_ascii_lowercase(needle_chars[ni]))
            {
                if(ni > 0)
                    si += ni - 1;
                break;
            }
            if(ni + 1 == needle.length())
                return true;
        }
    }
    return false;
}

bool is_whitespace(StringView str)
{
    return all_of(str, is_ascii_space);
}

StringView trim(StringView str, StringView characters, TrimMode mode)
{
    size_t substring_start = 0;
    size_t substring_length = str.length();

    if(mode == TrimMode::Left || mode == TrimMode::Both)
    {
        for(size_t i = 0; i < str.length(); ++i)
        {
            if(substring_length == 0)
                return "";
            if(!characters.contains(str[i]))
                break;
            ++substring_start;
            --substring_length;
        }
    }

    if(mode == TrimMode::Right || mode == TrimMode::Both)
    {
        for(size_t i = str.length() - 1; i > 0; --i)
        {
            if(substring_length == 0)
                return "";
            if(!characters.contains(str[i]))
                break;
            --substring_length;
        }
    }

    return str.substring_view(substring_start, substring_length);
}

Optional<size_t> find(StringView haystack, char needle, size_t start)
{
    if(start >= haystack.length())
        return {};
    for(size_t i = start; i < haystack.length(); ++i)
    {
        if(haystack[i] == needle)
            return i;
    }
    return {};
}

Optional<size_t> find(StringView haystack, StringView needle, size_t start)
{
    if(start > haystack.length())
        return {};
    auto index = Mods::memmem_optional(
        haystack.characters_without_null_termination() + start, haystack.length() - start, needle.characters_without_null_termination(), needle.length());
    return index.has_value() ? (*index + start) : index;
}

Optional<size_t> find_last(StringView haystack, char needle)
{
    for(size_t i = haystack.length(); i > 0; --i)
    {
        if(haystack[i - 1] == needle)
            return i - 1;
    }
    return {};
}

} // namespace Mods