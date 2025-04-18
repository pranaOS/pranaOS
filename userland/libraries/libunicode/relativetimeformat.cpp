/**
 * @file relativetimeformat.cpp
 * @author Krisna Pranav
 * @brief Relative Time Format
 * @version 6.0
 * @date 2024-11-06
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <libunicode/relativetimeformat.h>

namespace Unicode 
{
    /**
     * @param time_unit 
     * @return Optional<TimeUnit> 
     */
    Optional<TimeUnit> time_unit_from_string(StringView time_unit)
    {
        if (time_unit == "second"sv)
            return TimeUnit::Second;
        if (time_unit == "minute"sv)
            return TimeUnit::Minute;
        if (time_unit == "hour"sv)
            return TimeUnit::Hour;
        if (time_unit == "day"sv)
            return TimeUnit::Day;
        if (time_unit == "week"sv)
            return TimeUnit::Week;
        if (time_unit == "month"sv)
            return TimeUnit::Month;
        if (time_unit == "quarter"sv)
            return TimeUnit::Quarter;
        if (time_unit == "year"sv)
            return TimeUnit::Year;
        return {};
    }

    StringView time_unit_to_string(TimeUnit time_unit)
    {
        switch (time_unit) {
        case TimeUnit::Second:
            return "second"sv;
        case TimeUnit::Minute:
            return "minute"sv;
        case TimeUnit::Hour:
            return "hour"sv;
        case TimeUnit::Day:
            return "day"sv;
        case TimeUnit::Week:
            return "week"sv;
        case TimeUnit::Month:
            return "month"sv;
        case TimeUnit::Quarter:
            return "quarter"sv;
        case TimeUnit::Year:
            return "year"sv;
        default:
            VERIFY_NOT_REACHED();
        }
    }

    Vector<RelativeTimeFormat> __attribute__((weak)) get_relative_time_format_patterns(StringView, TimeUnit, StringView, Style) 
    { 
        return {}; 
    }
} // namespace Unicode
