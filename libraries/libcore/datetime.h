/**
 * @file datetime.h
 * @author Krisna Pranav
 * @brief date time
 * @version 6.0
 * @date 2023-09-22
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/string.h>
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
         * @param year 
         * @param month 
         * @param day 
         * @param hour 
         * @param minute 
         * @param second 
         */
        void set_time(unsigned year, unsigned month = 1, unsigned day = 0, unsigned hour = 0, unsigned minute = 0, unsigned second = 0);

        /**
         * @param format 
         * @return String 
         */
        String to_string(const String& format = "%Y-%m-%d %H:%M:%S") const;

        /**
         * @param year 
         * @param month 
         * @param day 
         * @param hour 
         * @param minute 
         * @param second 
         * @return DateTime 
         */
        static DateTime create(unsigned year, unsigned month = 1, unsigned day = 0, unsigned hour = 0, unsigned minute = 0, unsigned second = 0);

        /**
         * @return DateTime 
         */
        static DateTime now();

        /**
         * @return DateTime 
         */
        static DateTime from_timestamp(time_t);
        
        /**
         * @return true 
         * @return false 
         */
        bool is_before(const String&) const;

    private:
        time_t m_timestamp { 0 };
        
        unsigned m_year { 0 };
        unsigned m_month { 0 };
        unsigned m_day { 0 };
        unsigned m_hour { 0 };
        unsigned m_minute { 0 };
        unsigned m_second { 0 };
    };

    /**
     * @return const LogStream& 
     */
    const LogStream& operator<<(const LogStream&, const DateTime&);

}
