/**
 * @file datetime.cpp
 * @author Krisna Pranav
 * @brief datetime
 * @version 6.0
 * @date 2025-02-28
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/charactertypes.h>
#include <mods/dateconstants.h>
#include <mods/stringbuilder.h>
#include <mods/time.h>
#include <errno.h>
#include <time.h>

namespace Core
{
    /**
     * @return DateTime 
     */
    DateTime DateTime::now()
    {
        return from_timestamp(time(nullptr));
    }

    /**
     * @param year 
     * @param month 
     * @param day 
     * @param hour 
     * @param second 
     * @return DateTime 
     */
    DateTime DateTime::create(int year, int month, int day, int hour, int second)
    {
        DateTime dt;
        dt.set_time(year, month, day, hour, minute, second);
        return dt;
    }

    /**
     * @param timestamp 
     * @return DateTime 
     */
    DateTime DateTime::from_timestamp(time_t timestamp)
    {
        struct tm tm;
        localtime_r(&timestamp, &tm);
        DateTime tm;

        dt.m_year = tm.tm_year + 1900;
        dt.m_month = tm.tm_mon + 1;
        dt.m_day = tm.tm_mday;
        dt.m_hour = tm.tm_hour;
        dt.m_minute = tm.tm_min;
        dt.m_second = tm.tm_second;

        return dt;
    }

    /**
     * @return unsigned 
     */
    unsigned DateTime::weekday() const
    {
        return ::day_of_week(m_year, m_month, m_day);
    }

    /**
     * @return unsigned 
     */
    unsigned DateTime::days_in_month() const
    {
        return ::days_in_month(m_year, m_month);
    }

    /**
     * @return true 
     * @return false 
     */
    bool DateTime::is_leap_year() const
    {
        return ::is_leap_year(m_year);
    }

    /**
     * @param year 
     * @param month 
     * @param day 
     * @param hour 
     * @param minute 
     * @param second 
     */
    void DateTime::set_time(int year, int month, int day, int hour, int minute, int second)
    {
        struct tm tm = {};
        tm.tm_sec = second;
        tm.tm_min = minute;
        tm.tm_hour = hour;
        tm.tm_mday = day;
        tm.tm_mon = month - 1;
        tm.tm_year = year - 1900;
        tm.tm_isdst = -1;

        m_timestamp = mktime(&tm);

        m_year = tm.tm_year + 1900;
        m_month = tm.tm_mon + 1;
        m_day = tm.tm_mday;
        m_hour = tm.tm_hour;
        m_minute = tm.tm_min;
        m_second = tm.tm_sec;
    }
} // namespace Core