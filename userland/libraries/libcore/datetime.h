/**
 * @file datetime.h
 * @author Krisna Pranav
 * @brief date time
 * @version 6.0
 * @date 2023-09-22
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/string.h>
#include <mods/stringview.h>
#include <libipc/forward.h>
#include <time.h>

namespace Core 
{

    class DateTime 
    {
    public:
        /**
         * @return time_t 
         */
        time_t timestamp() const 
        { 
            return m_timestamp; 
        }

        /**
         * @return unsigned 
         */
        unsigned year() const 
        { 
            return m_year; 
        }

        unsigned month() const 
        { 
            return m_month; 
        }
        
        unsigned day() const 
        { 
            return m_day; 
        }

        unsigned hour() const 
        { 
            return m_hour; 
        }

        unsigned minute() const 
        { 
            return m_minute; 
        }

        unsigned second() const 
        { 
            return m_second; 
        }

        unsigned weekday() const;
        unsigned days_in_month() const;
        unsigned day_of_year() const;
        bool is_leap_year() const;

        /**
         * @brief Set the time object
         * 
         * @param year 
         * @param month 
         * @param day 
         * @param hour 
         * @param minute 
         * @param second 
         */
        void set_time(int year, int month = 1, int day = 1, int hour = 0, int minute = 0, int second = 0);

        /**
         * @param format 
         * @return String 
         */
        String to_string(StringView format = "%Y-%m-%d %H:%M:%S"sv) const;

        /**
         * @param year 
         * @param month 
         * @param day 
         * @param hour 
         * @param minute 
         * @param second 
         * @return DateTime 
         */
        static DateTime create(int year, int month = 1, int day = 1, int hour = 0, int minute = 0, int second = 0);

        /**
         * @return DateTime 
         */
        static DateTime now();

        /**
         * @return DateTime 
         */
        static DateTime from_timestamp(time_t);

        /**
         * @param format 
         * @param string 
         * @return Optional<DateTime> 
         */
        static Optional<DateTime> parse(StringView format, String const& string);

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator<(DateTime const& other) const 
        { 
            return m_timestamp < other.m_timestamp; 
        }

    private:
        time_t m_timestamp { 0 };
        int m_year { 0 };
        int m_month { 0 };
        int m_day { 0 };
        int m_hour { 0 };
        int m_minute { 0 };
        int m_second { 0 };
    }; // class DateTime

} // namespace Core

namespace IPC 
{

    /**
     * @return true 
     * @return false 
     */
    bool encode(Encoder&, Core::DateTime const&);

    /**
     * @return ErrorOr<void> 
     */
    ErrorOr<void> decode(Decoder&, Core::DateTime&);

} // namespace IPC
