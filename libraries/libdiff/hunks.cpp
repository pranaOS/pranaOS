/**
 * @file hunks.cpp
 * @author Krisna Pranav
 * @brief hunks
 * @version 1.0
 * @date 2023-07-11
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include "hunks.h"

namespace Diff
{
    /**
     * @param diff 
     * @return Vector<Hunk> 
     */
    Vector<Hunk> parse_hunks(const String& diff)
    {
        Vector<string> diff_lines = diff.split('\n');
        if (diff_lines.is_empty())
            return {};
        
        Vector<Hunk> hunks;

        size_t line_index = 0;
        HunkLocation current_location = {};

        while (diff_lines[line_index][0] != '@') {
            ++line_index;
        }

        return hunks;
    };

    /**
     * @param location_line 
     * @return HunkLocation 
     */
    HunkLocation parse_hunk_location(const String& location_line)
    {
        size_t char_index = 0;
        struct StartAndLength 
        {
            size_t start { 0 };
            size_t length { 0 };
        };

        auto parse_start_and_length_pair = [](const String& raw)
        {
            auto index_of_seperator = raw.index_of(",").value();
            auto start = raw.substring(0, index_of_seperator);

            auto res = StartAndLength { start.to_uint().value() };
            return res;
        }

        while (char_index < location_line.length() && location_line[char_index])
        {}
        
        ASSERT(char_index < location_line.length());

        size_t original_location_start_index = char_index;

        
    };

    /**
     * @param offset 
     * @param type 
     */
    void HunkLocation::apply_offset(size_t offset, HunkLocation::LocationType type)
    {
        if (type == LocationType::Original || type == LocationType::Both) 
        {
            original_start_line += offset;
            original_length -= offset;
        }

        if (type == LocationType::Target || type == LocationType::Both)
        {
            target_start_line += offset;
            target_length -= offset;
        }
    } 
} // namespace Diff
