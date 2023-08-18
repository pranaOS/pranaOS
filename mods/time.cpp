/**
 * @file time.cpp
 * @author Krisna Pranav
 * @brief Time
 * @version 6.0
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

    /**
     * @param year 
     * @param month 
     * @return int 
     */
    int days_in_month(int year, unsigned month) {
        ASSERT(month >= 1 && month <= 12);
        if (month == 2)
            return is_leap_year(year) ? 29 : 28;

        bool is_long_month = (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12);
        return is_long_month ? 31 : 30;
    }

    /** 
     * @param year 
     * @param month 
     * @param day 
     * @return unsigned 
     */
    unsigned day_of_week(int year, unsigned month, int day) {
        ASSERT(month >= 1 && month <= 12);
        static const int seek_table[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
        if (month < 3)
            --year;

        return (year + year / 4 - year / 100 + year / 400 + seek_table[month - 1] + day) % 7;
    }
}
