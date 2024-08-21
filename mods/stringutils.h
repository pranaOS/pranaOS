/**
 * @file stringutils.h
 * @author Krisna Pranav
 * @brief StringUtils
 * @version 6.0
 * @date 2023-06-28
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "forward.h"

namespace Mods {

    /**
     * @brief CaseSensitivity
     * 
     */
    enum class CaseSensitivity {
        CaseInsensitive,
        CaseSensitive,
    };

    // TrimMode
    enum class TrimMode {
        Left,
        Right,
        Both
    };

    // MarkSpan
    struct MaskSpan {
        size_t start;
        size_t length;

        /**
         * @brief ==
         * 
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(const MaskSpan& other) const {
            return start == other.start && length == other.length;
        }

        /**
         * @brief !=
         * 
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(const MaskSpan& other) const
        {
            return !(*this == other);
        }
    };

    namespace StringUtils {
        /**
         * @brief matches
         * 
         * @param str 
         * @param mask 
         * @param match_spans 
         * @return true 
         * @return false 
         */
        bool matches(const StringView& str, const StringView& mask, CaseSensitivity = CaseSensitivity::CaseInsensitive, Vector<MaskSpan>* match_spans = nullptr);

        /**
         * @brief convert_to_int
         * 
         * @return Optional<int> 
         */
        Optional<int> convert_to_int(const StringView&);

        /**
         * @brief convert_to_uint
         * 
         * @return Optional<unsigned> 
         */
        Optional<unsigned> convert_to_uint(const StringView&);

        /**
         * @brief convert_to_uint_from_hex
         * 
         * @return Optional<unsigned> 
         */
        Optional<unsigned> convert_to_uint_from_hex(const StringView&);

        /**
         * @brief equals_ignoring_case
         * 
         * @return true 
         * @return false 
         */
        bool equals_ignoring_case(const StringView&, const StringView&);

        /**
         * @brief ends_with
         * 
         * @param a 
         * @param b 
         * @return true 
         * @return false 
         */
        bool ends_with(const StringView& a, const StringView& b, CaseSensitivity);

        /**
         * @brief starts_with
         * 
         * @return true 
         * @return false 
         */
        bool starts_with(const StringView&, const StringView&, CaseSensitivity);

        /**
         * @brief contains
         * 
         * @return true 
         * @return false 
         */
        bool contains(const StringView&, const StringView&, CaseSensitivity);

        /**
         * @brief trim_whitespace
         * 
         * @param mode 
         * @return StringView 
         */
        StringView trim_whitespace(const StringView&, TrimMode mode);
    }

}

// using mods
using Mods::CaseSensitivity;
using Mods::TrimMode;