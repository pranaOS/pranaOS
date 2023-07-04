/**
 * @file string_utils.h
 * @author Krisna Pranav
 * @brief string utils
 * @version 1.0
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "forward.h"

namespace Mods {
    
    enum class CaseSensitivity {
        CaseInsensitive,
        CaseSensitive,
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
        bool matches(const StringView& str, const StringView& mask, CaseSensitivity = CaseSensitivity::CaseSensitive);
    }

} // namespace Mods
