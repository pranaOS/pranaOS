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
        "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
    };

    static constexpr Array<StringView, 7> short_day_names = {
        "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
    };

    static constexpr Array<StringView, 7> mini_day_names = {
        "Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"
    };

    static constexpr Array<StringView, 7> micro_day_names = {
        "S", "M", "T", "W", "T", "F", "S"
    };

    static constexpr Array<StringView, 12> long_month_names = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };

    static constexpr Array<StringView, 12> short_month_names = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };
}

using Mods::long_day_names;
using Mods::long_month_names;
using Mods::micro_day_names;
using Mods::mini_day_names;
using Mods::short_day_names;
using Mods::short_month_names;
