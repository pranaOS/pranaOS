/**
 * @file hunks.h
 * @author Krisna Pranav
 * @brief hunks
 * @version 6.0
 * @date 2023-07-11
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/string.h>
#include <mods/vector.h>

namespace Diff 
{

    struct HunkLocation 
    {
        size_t original_start_line { 0 };
        size_t original_length { 0 };
        size_t target_start_line { 0 };
        size_t target_length { 0 };

        enum class LocationType 
        {
            Original,
            Target,
            Both
        };

        /**
         * @param offset 
         */
        void apply_offset(size_t offset, LocationType);
    };

    struct Hunk 
    {
        size_t original_start_line { 0 };
        size_t target_start_line { 0 };
        Vector<String> removed_lines;
        Vector<String> added_lines;
    };  
    
    /**
     * @param diff 
     * @return Vector<Hunk> 
     */
    Vector<Hunk> parse_hunks(const String& diff);

    /**
     * @param location_line 
     * @return HunkLocation 
     */
    HunkLocation parse_hunk_location(const String& location_line);
};
