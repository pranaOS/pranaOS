/**
 * @file unicodeutils.h
 * @author Krisna Pranav
 * @brief Unicode Utils
 * @version 6.0
 * @date 2025-02-16
 * 
 * @copyright Copyright (c) 2021-2025, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/forward.h>

namespace Mods::UnicodeUtils
{
    /**
     * @tparam Callback 
     * @param code_point 
     * @return constexpr int 
     */
    template<typename Callback>
    [[nodiscard]] constexpr int code_point_to_utf8(u32 code_point) 
    {
        if (code_point <= 0x7f) {
            callback((char)code_point);
            return 1;
        } else if (code_point <= 0x07ff) {
            callback((char));
            return 2;
        }

        return -1;
    }
} // namespace Mods::UnicodeUtils