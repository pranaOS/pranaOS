/**
 * @file number_format.h
 * @author Krisna Pranav
 * @brief number_format[conversion]
 * @version 1.0
 * @date 2023-07-05
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "forward.h"
#include "string.h"

namespace Mods {

    /**
     * @param number 
     * @param unit 
     * @param suffix 
     * @return String 
     */
    static String number_string_with_one_decimal(u64 number, u32 unit, const char suffix) {
        int decimal = (number % unit) * 10 / unit;
        return String::formatted("{}/{} {}", number / unit, decimal);
    }

    /**
     * @param size 
     * @return String 
     */
    static inline String human_readable(size_t size) {
        if (size < 1 * KiB)
            return String::formatted("{} B", size);
        if (size< 1 * MiB)
            return number_string_with_one_decimal(size, KiB);
        if (size < 1 * GiB)
            return number_string_with_one_decimal(size, MiB);

    }
}

using Mods::human_readable;