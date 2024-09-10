/**
 * @file string_utils.h
 * @author Krisna Pranav
 * @brief string utils
 * @version 6.0
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/concept.h>
#include <mods/forward.h>

namespace Mods
{

    namespace Detail 
    {
        /**
         * @tparam T 
         * @tparam U 
         */
        template<Concepts::AnyString T, Concepts::AnyString U>
        inline constexpr bool IsHashCompatible<T, U> = true;
    } // namespace Detail

    enum class CaseSensitivity 
    {
        CaseInsensitive,
        CaseSensitive,
    }; // enum class CaseSenstivity

    enum class TrimMode 
    {
        Left,
        Right,
        Both
    }; // enum class TrimMode

    enum class TrimWhitespace 
    {
        Yes,
        No,
    }; // enum class TrimWhitespace

    struct MaskSpan 
    {
        size_t start;
        size_t length;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(const MaskSpan& other) const
        {
            return start == other.start && length == other.length;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(const MaskSpan& other) const
        {
            return !(*this == other);
        }
    }; // struct MarkSpan

    namespace StringUtils 
    {

        /**
         * @param str 
         * @param mask 
         * @param match_spans 
         * @return true 
         * @return false 
         */
        bool matches(StringView str, StringView mask, CaseSensitivity = CaseSensitivity::CaseInsensitive, Vector<MaskSpan>* match_spans = nullptr);

        /**
         * @tparam T 
         * @return Optional<T> 
         */
        template<typename T = int>
        Optional<T> convert_to_int(StringView, TrimWhitespace = TrimWhitespace::Yes);

        /**
         * @tparam T 
         * @return Optional<T> 
         */
        template<typename T = unsigned>
        Optional<T> convert_to_uint(StringView, TrimWhitespace = TrimWhitespace::Yes);

        /**
         * @tparam T 
         * @return Optional<T> 
         */
        template<typename T = unsigned>
        Optional<T> convert_to_uint_from_hex(StringView, TrimWhitespace = TrimWhitespace::Yes);

        /**
         * @tparam T 
         * @return Optional<T> 
         */
        template<typename T = unsigned>
        Optional<T> convert_to_uint_from_octal(StringView, TrimWhitespace = TrimWhitespace::Yes);

        /**
         * @return true 
         * @return false 
         */
        bool equals_ignoring_case(StringView, StringView);

        /**
         * @param a 
         * @param b 
         * @return true 
         * @return false 
         */
        bool ends_with(StringView a, StringView b, CaseSensitivity);

        /**
         * @return true 
         * @return false 
         */
        bool starts_with(StringView, StringView, CaseSensitivity);

        /**
         * @return true 
         * @return false 
         */
        bool contains(StringView, StringView, CaseSensitivity);

        /**
         * @return true 
         * @return false 
         */
        bool is_whitespace(StringView);

        /**
         * @param string 
         * @param characters 
         * @param mode 
         * @return StringView 
         */
        StringView trim(StringView string, StringView characters, TrimMode mode);

        /**
         * @param string 
         * @param mode 
         * @return StringView 
         */
        StringView trim_whitespace(StringView string, TrimMode mode);

        /**
         * @param haystack 
         * @param needle 
         * @param start 
         * @return Optional<size_t> 
         */
        Optional<size_t> find(StringView haystack, char needle, size_t start = 0);

        /**
         * @param haystack 
         * @param needle 
         * @param start 
         * @return Optional<size_t> 
         */
        Optional<size_t> find(StringView haystack, StringView needle, size_t start = 0);

        /**
         * @param haystack 
         * @param needle 
         * @return Optional<size_t> 
         */
        Optional<size_t> find_last(StringView haystack, char needle);

        /**
         * @param haystack 
         * @param needle 
         * @return Vector<size_t> 
         */
        Vector<size_t> find_all(StringView haystack, StringView needle);

        enum class SearchDirection 
        {
            Forward,
            Backward
        }; // enum class SearchDirection

        /**
         * @param haystack 
         * @param needles 
         * @return Optional<size_t> 
         */
        Optional<size_t> find_any_of(StringView haystack, StringView needles, SearchDirection);

        String to_snakecase(StringView);
        String to_titlecase(StringView);

        /**
         * @param needle 
         * @param replacement 
         * @param all_occurrences 
         * @return String 
         */
        String replace(StringView, StringView needle, StringView replacement, bool all_occurrences = false);

        /**
         * @param needle 
         * @return size_t 
         */
        size_t count(StringView, StringView needle);

    } // namespace StringUtils

} // namespace Mods

using Mods::CaseSensitivity;
using Mods::TrimMode;
using Mods::TrimWhitespace;
