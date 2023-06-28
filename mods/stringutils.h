/**
 * @file stringutils.h
 * @author Krisna Pranav
 * @brief StringUtils
 * @version 1.0
 * @date 2023-06-28
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "forward.h"

namespace Mods {
    enum class CaseSensitivity {
        CaseInsensitive,
        Casesensitive,
    };

    enum class TrimMode {
        Left,
        Right,
        Both
    };


    struct MarkSpan {
        size_t start;
        size_t length;

        bool operator==(const MarkSpan& other) const {
            return start == other.start && length == other.length;
        }

        bool operator!=(const MarkSpan& other) const {
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
         * @brief trim_whitespaces
         * 
         * @param mode 
         * @return StringView 
         */
        StringView trim_whitespaces(const StringView&, TrimMode mode);
    }
}