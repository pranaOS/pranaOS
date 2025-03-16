/**
 * @file dateconstants.h
 * @author Krisna Pranav
 * @brief Date Constants
 * @version 6.0
 * @date 2025-02-04
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/array.h>
#include <mods/stringview.h>

namespace Mods 
{

    static constexpr Array<StringView, 7> long_day_names = {
        "Sunday"sv, "Monday"sv, "Tuesday"sv, "Wednesday"sv, "Thursday"sv, "Friday"sv, "Saturday"sv
    };

    static constexpr Array<StringView, 7> short_day_names = {
        "Sun"sv, "Mon"sv, "Tue"sv, "Wed"sv, "Thu"sv, "Fri"sv, "Sat"sv
    };

    static constexpr Array<StringView, 7> mini_day_names = {
        "Su"sv, "Mo"sv, "Tu"sv, "We"sv, "Th"sv, "Fr"sv, "Sa"sv
    };

    static constexpr Array<StringView, 7> micro_day_names = {
        "S"sv, "M"sv, "T"sv, "W"sv, "T"sv, "F"sv, "S"sv
    };

    static constexpr Array<StringView, 12> long_month_names = {
        "January"sv, "February"sv, "March"sv, "April"sv, "May"sv, "June"sv,
        "July"sv, "August"sv, "September"sv, "October"sv, "November"sv, "December"sv
    };

    static constexpr Array<StringView, 12> short_month_names = {
        "Jan"sv, "Feb"sv, "Mar"sv, "Apr"sv, "May"sv, "Jun"sv,
        "Jul"sv, "Aug"sv, "Sep"sv, "Oct"sv, "Nov"sv, "Dec"sv
    };

}

#if USING_MODS_GLOBALLY
using Mods::long_day_names;
using Mods::long_month_names;
using Mods::micro_day_names;
using Mods::mini_day_names;
using Mods::short_day_names;
using Mods::short_month_names;
#endif
