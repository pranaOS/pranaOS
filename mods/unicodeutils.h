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
     * @param callback 
     * @return constexpr int 
     */
    template <typename Callback>
    [[nodiscard]] constexpr int code_point_to_utf8(u32 code_point, Callback callback)
    {
        if(code_point <= 0x7f)
        {
            callback((char)code_point);
            return 1;
        }
        else if(code_point <= 0x07ff)
        {
            callback((char)(((code_point >> 6) & 0x1f) | 0xc0));
            callback((char)(((code_point >> 0) & 0x3f) | 0x80));
            return 2;
        }
        else if(code_point <= 0xffff)
        {
            callback((char)(((code_point >> 12) & 0x0f) | 0xe0));
            callback((char)(((code_point >> 6) & 0x3f) | 0x80));
            callback((char)(((code_point >> 0) & 0x3f) | 0x80));
            return 3;
        }
        else if(code_point <= 0x10ffff)
        {
            callback((char)(((code_point >> 18) & 0x07) | 0xf0));
            callback((char)(((code_point >> 12) & 0x3f) | 0x80));
            callback((char)(((code_point >> 6) & 0x3f) | 0x80));
            callback((char)(((code_point >> 0) & 0x3f) | 0x80));
            return 4;
        }
        return -1;
    }
} // namespace Mods::UnicodeUtils