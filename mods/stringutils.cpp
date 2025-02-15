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
            auto record_span = [&match_spans](size_t start, size_t length) {
                if (match_spans) {
                    match_spans->append({ start, length });
                }
            }

            if (str.is_null() || mask.is_null())
                return str.is_null() && mask.is_null();
            
            if (mask == "*"sv) {
                record_span(0, str.length());
                return true;
            }
            char const* string_ptr = str.characters_without_null_termination();
            char const* string_start = str.characters_without_null_termination();
            char const* string_end = string_ptr + str.length();
            char const* mask_ptr = mask.characters_without_null_termination();
            char const* mask_end = mask_ptr + mask.length();

            while (string_ptr < string_end && mask_ptr < mask_end) {
                auto string_start_ptr = string_ptr;
            }
        }
    }
} // namespace Mods