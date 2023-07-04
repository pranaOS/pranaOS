/**
 * @file time.cpp
 * @author Krisna Pranav
 * @brief Time
 * @version 1.0
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include "assertions.h"
#include "time.h"

namespace Mods {
    /**
     * @param year 
     * @param month 
     * @param day 
     * @return int 
     */
    int day_of_year(int year, unsigned month, int day) {
        ASSERT(month >= 1 && month <= 12);

        static const int seek_table[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
        int day_of_year = seek_table[month - 1] + day - 1;

        if (is_leap_year(year) && month >= 3)
            day_of_year++;

        return day_of_year;
    }
}