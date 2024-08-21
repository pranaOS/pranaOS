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

#include "forward.h"

namespace Mods {

    /// @brief CaseSensitivity
    enum class CaseSensitivity {
        CaseInsensitive,
        CaseSensitive,
    };

    /// @brief TrimMode[Left, Right, Both]
    enum class TrimMode {
        Left,
        Right,
        Both
    };

    struct MaskSpan {
        size_t start;
        size_t length;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(const MaskSpan& other) const {
            return start == other.start && length == other.length;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(const MaskSpan& other) const {
            return !(*this == other);
        }
    };

    namespace StringUtils {
        
        /**
         * @param str 
         * @param mask 
         * @param match_spans 
         * @return true 
         * @return false 
         */
        bool matches(const StringView& str, const StringView& mask, CaseSensitivity = CaseSensitivity::CaseInsensitive, Vector<MaskSpan>* match_spans = nullptr);

        /** 
         * @return Optional<int> 
         */
        Optional<int> convert_to_int(const StringView&);

        /**
         * @return Optional<unsigned> 
         */
        Optional<unsigned> convert_to_uint(const StringView&);

        /**
         * @return Optional<unsigned> 
         */
        Optional<unsigned> convert_to_uint_from_hex(const StringView&);

        /**
         * @return true 
         * @return false 
         */
        bool equals_ignoring_case(const StringView&, const StringView&);
        
        /**
         * @param a 
         * @param b 
         * @return true 
         * @return false 
         */
        bool ends_with(const StringView& a, const StringView& b, CaseSensitivity);

        /**
         * @return true 
         * @return false 
         */
        bool starts_with(const StringView&, const StringView&, CaseSensitivity);

        /**
         * @return true 
         * @return false 
         */
        bool contains(const StringView&, const StringView&, CaseSensitivity);

        /**
         * @param mode 
         * @return StringView 
         */
        StringView trim_whitespace(const StringView&, TrimMode mode);
    }

}

using Mods::CaseSensitivity;
using Mods::TrimMode;
