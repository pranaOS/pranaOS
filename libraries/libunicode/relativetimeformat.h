/**
 * @file relativetimeformat.h
 * @author Krisna Pranav
 * @brief Relative Time Format
 * @version 6.0
 * @date 2024-11-06
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/optional.h>
#include <mods/string_view.h>
#include <mods/vector.h>
#include <libunicode/forward.h>
#include <libunicode/locale.h>

namespace Unicode 
{
    enum class TimeUnit 
    {
        Second,
        Minute,
        Hour,
        Day,
        Week,
        Month,
        Quarter,
        Year,
    }; // enum class TimeUnit 

    struct RelativeTimeFormat 
    {
        enum class Plurality 
        {
            Zero,
            One,
            Two,
            Few,
            Many,
            Other,
        }; // enum class Plurality 

        Plurality plurality { Plurality::Other };
        StringView pattern;
    }; // struct RelativeTimeFormat

    /**
     * @param time_unit 
     * @return Optional<TimeUnit> 
     */
    Optional<TimeUnit> time_unit_from_string(StringView time_unit);

    /**
     * @param time_unit 
     * @return StringView 
     */
    StringView time_unit_to_string(TimeUnit time_unit);

    /**
     * @brief Get the relative time format patterns object
     * 
     * @param locale 
     * @param time_unit 
     * @param tense_or_number 
     * @param style 
     * @return Vector<RelativeTimeFormat> 
     */
    Vector<RelativeTimeFormat> get_relative_time_format_patterns(StringView locale, TimeUnit time_unit, StringView tense_or_number, Style style);

} // namespace Unicode