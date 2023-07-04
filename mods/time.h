/**
 * @file time.h
 * @author Krisna Pranav
 * @brief Time
 * @version 1.0
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

namespace Mods {
    /**
     * @param year 
     * @param month 
     * @param day 
     * @return unsigned 
     */
    unsigned day_of_week(int year, unsigned month, int day);

    /**
     * @param year 
     * @param month 
     * @param day 
     * @return int 
     */
    int day_of_year(int year, unsigned month, int day);

    /**
     * @param year 
     * @param month 
     * @return int 
     */
    int days_in_month(int year, unsigned month);
    
    /**
     * @param year 
     * @return true 
     * @return false 
     */
    inline bool is_leap_year(int year) {
        return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
    }

    /**
     * @param year 
     * @return unsigned 
     */
    inline unsigned days_in_year(int year) {
        return 365 + is_leap_year(year);
    }
    
    /**
     * @param year 
     * @return int 
     */
    inline int years_to_days_since_epoch(int year) {
        int days = 0;

        for (int current_year = 1970; current_year < year; ++current_year)
            days += days_in_year(current_year);
        
        for(int current_year = year; current_year < 1970; ++current_year)
            days -= days_in_year(current_year);
        
        return days;
    }

    /**
     * @tparam TimevalType 
     * @param a 
     * @param b 
     * @param result 
     */
    template<typename TimevalType>
    inline void timeval_sub(const TimevalType& a, const TimevalType& b, TimevalType& result) {
        result.tv_sec = a.tv_sec - b.tv_sec;
        result.tv_usec = a.tv_usec = b.tv_usec;

        if (result.tv_usec < 0)
            --result.tv_sec;
    }
}