/**
 * @file time.h
 * @author Krisna Pranav
 * @brief Time
 * @version 6.0
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
        for (int current_year = year; current_year < 1970; ++current_year)
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
        result.tv_usec = a.tv_usec - b.tv_usec;
        if (result.tv_usec < 0) {
            --result.tv_sec;
            result.tv_usec += 1'000'000;
        }
    }   

    /**
     * @tparam TimevalType 
     * @param a 
     * @param b 
     * @param result 
     */
    template<typename TimevalType>
    inline void timeval_add(const TimevalType& a, const TimevalType& b, TimevalType& result) {
        result.tv_sec = a.tv_sec + b.tv_sec;
        result.tv_usec = a.tv_usec + b.tv_usec;
        if (result.tv_usec >= 1'000'000) {
            ++result.tv_sec;
            result.tv_usec -= 1'000'000;
        }
    }   

    /**
     * @tparam TimespecType 
     * @param a 
     * @param b 
     * @param result 
     */
    template<typename TimespecType>
    inline void timespec_sub(const TimespecType& a, const TimespecType& b, TimespecType& result) {
        result.tv_sec = a.tv_sec - b.tv_sec;
        result.tv_nsec = a.tv_nsec - b.tv_nsec;
        if (result.tv_nsec < 0) {
            --result.tv_sec;
            result.tv_nsec += 1'000'000'000;
        }
    }

    /**
     * @tparam TimespecType 
     * @tparam TimevalType 
     * @param a 
     * @param b 
     * @param result 
     */
    template<typename TimespecType, typename TimevalType>
    inline void timespec_sub_timeval(const TimespecType& a, const TimevalType& b, TimespecType& result) {
        result.tv_sec = a.tv_sec - b.tv_sec;
        result.tv_nsec = a.tv_nsec - b.tv_usec * 1000;
        if (result.tv_nsec < 0) {
            --result.tv_sec;
            result.tv_nsec += 1'000'000'000;
        }
    }   

    /**
     * 
     * @tparam TimespecType 
     * @param a 
     * @param b 
     * @param result 
     */
    template<typename TimespecType>
    inline void timespec_add(const TimespecType& a, const TimespecType& b, TimespecType& result) {
        result.tv_sec = a.tv_sec + b.tv_sec;
        result.tv_nsec = a.tv_nsec + b.tv_nsec;
        if (result.tv_nsec >= 1000'000'000) {
            ++result.tv_sec;
            result.tv_nsec -= 1000'000'000;
        }
    }

    /**
     * @tparam TimespecType 
     * @tparam TimevalType 
     * @param a 
     * @param b 
     * @param result 
     */
    template<typename TimespecType, typename TimevalType>
    inline void timespec_add_timeval(const TimespecType& a, const TimevalType& b, TimespecType& result) {
        result.tv_sec = a.tv_sec + b.tv_sec;
        result.tv_nsec = a.tv_nsec + b.tv_usec * 1000;
        if (result.tv_nsec >= 1000'000'000) {
            ++result.tv_sec;
            result.tv_nsec -= 1000'000'000;
        }
    }

    /**
     * @tparam TimevalType 
     * @tparam TimespecType 
     * @param tv 
     * @param ts 
     */
    template<typename TimevalType, typename TimespecType>
    inline void timeval_to_timespec(const TimevalType& tv, TimespecType& ts) {
        ts.tv_sec = tv.tv_sec;
        ts.tv_nsec = tv.tv_usec * 1000;
    }

    /**
     * @tparam TimespecType 
     * @tparam TimevalType 
     * @param ts 
     * @param tv 
     */
    template<typename TimespecType, typename TimevalType>
    inline void timespec_to_timeval(const TimespecType& ts, TimevalType& tv) {
        tv.tv_sec = ts.tv_sec;
        tv.tv_usec = ts.tv_nsec / 1000;
    }

    /**
     * @tparam TimespecType 
     * @param a 
     * @param b 
     * @return true 
     * @return false 
     */
    template<typename TimespecType>
    inline bool operator>=(const TimespecType& a, const TimespecType& b) {
        return a.tv_sec > b.tv_sec || (a.tv_sec == b.tv_sec && a.tv_nsec >= b.tv_nsec);
    }

    /**
     * @tparam TimespecType 
     * @param a 
     * @param b 
     * @return true 
     * @return false 
     */
    template<typename TimespecType>
    inline bool operator>(const TimespecType& a, const TimespecType& b) {
        return a.tv_sec > b.tv_sec || (a.tv_sec == b.tv_sec && a.tv_nsec > b.tv_nsec);
    }

    /**
     * @tparam TimespecType 
     * @param a 
     * @param b 
     * @return true 
     * @return false 
     */
    template<typename TimespecType>
    inline bool operator<(const TimespecType& a, const TimespecType& b) {
        return a.tv_sec < b.tv_sec || (a.tv_sec == b.tv_sec && a.tv_nsec < b.tv_nsec);
    }

    /**
     * @tparam TimespecType 
     * @param a 
     * @param b 
     * @return true 
     * @return false 
     */
    template<typename TimespecType>
    inline bool operator<=(const TimespecType& a, const TimespecType& b) {
        return a.tv_sec < b.tv_sec || (a.tv_sec == b.tv_sec && a.tv_nsec <= b.tv_nsec);
    }

    /**
     * @tparam TimespecType 
     * @param a 
     * @param b 
     * @return true 
     * @return false 
     */
    template<typename TimespecType>
    inline bool operator==(const TimespecType& a, const TimespecType& b) {
        return a.tv_sec == b.tv_sec && a.tv_nsec == b.tv_nsec;
    }

    /**
     * @tparam TimespecType 
     * @param a 
     * @param b 
     * @return true 
     * @return false 
     */
    template<typename TimespecType>
    inline bool operator!=(const TimespecType& a, const TimespecType& b) {
        return a.tv_sec != b.tv_sec || a.tv_nsec != b.tv_nsec;
    }

}

using Mods::day_of_week;
using Mods::day_of_year;
using Mods::days_in_month;
using Mods::days_in_year;
using Mods::is_leap_year;
using Mods::timespec_add;
using Mods::timespec_add_timeval;
using Mods::timespec_sub;
using Mods::timespec_sub_timeval;
using Mods::timespec_to_timeval;
using Mods::timeval_add;
using Mods::timeval_sub;
using Mods::timeval_to_timespec;
using Mods::years_to_days_since_epoch;
using Mods::operator<=;
using Mods::operator<;
using Mods::operator>;
using Mods::operator>=;
using Mods::operator==;
using Mods::operator!=;
