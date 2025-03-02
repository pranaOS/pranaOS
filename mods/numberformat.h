/**
 * @file number_format.h
 * @author Krisna Pranav
 * @brief number_format[conversion]
 * @version 6.0
 * @date 2023-07-05
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/string.h>

namespace Mods
{
    /**
     * @param number 
     * @param unit 
     * @param suffix 
     * @return String 
     */
    static String number_string_with_one_decimal(u64 number, u64 unit, char const* suffix)
    {
        int decimal = (number % unit) * 10 / unit;
        return String::formatted("{}.{} {}", number / unit, decimal, suffix);
    }

    /**
     * @param size 
     * @return String 
     */
    static inline String human_readable_size(u64 size)
    {
        if(size < 1 * KiB)
            return String::formatted("{} B", size);
        if(size < 1 * MiB)
            return number_string_with_one_decimal(size, KiB, "KiB");
        if(size < 1 * GiB)
            return number_string_with_one_decimal(size, MiB, "MiB");
        if(size < 1 * TiB)
            return number_string_with_one_decimal(size, GiB, "GiB");
        if(size < 1 * PiB)
            return number_string_with_one_decimal(size, TiB, "TiB");
        if(size < 1 * EiB)
            return number_string_with_one_decimal(size, PiB, "PiB");
        return number_string_with_one_decimal(size, EiB, "EiB");
    }

    /**
     * @param size 
     * @return String 
     */
    static inline String human_readable_size_long(u64 size)
    {
        if(size < 1 * KiB)
            return String::formatted("{} bytes", size);
        else
            return String::formatted("{} ({} bytes)", human_readable_size(size), size);
    }

    /**
     * @param time_in_seconds 
     * @return String 
     */
    static inline String human_readable_time(i64 time_in_seconds)
    {
        auto hours = time_in_seconds / 3600;
        time_in_seconds = time_in_seconds % 3600;

        auto minutes = time_in_seconds / 60;
        time_in_seconds = time_in_seconds % 60;

        StringBuilder builder;

        if(hours > 0)
            builder.appendff("{} hour{} ", hours, hours == 1 ? "" : "s");

        if(minutes > 0)
            builder.appendff("{} minute{} ", minutes, minutes == 1 ? "" : "s");

        builder.appendff("{} second{}", time_in_seconds, time_in_seconds == 1 ? "" : "s");

        return builder.to_string();
    }

} // namespace Mods

using Mods::human_readable_size;
using Mods::human_readable_size_long;
using Mods::human_readable_time;