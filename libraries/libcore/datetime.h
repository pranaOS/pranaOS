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
        time_t timestamp() const 
        {
            return m_timestamp;
        }

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

        String to_string(const String& format = "%Y-%m-%d %H:%M:%S") const;


    private:
        time_t m_timestamp { 0 };

        unsigned m_year { 0 };
        unsigned m_month { 0 };
        unsigned m_day { 0 };
        unsigned m_hour { 0 };
        unsigned m_minute { 0 };
        unsigned m_second { 0 };
    }; // class DateTime

    const LogStream& operator<<(const LogStream&, const DateTime&);

} // namespace Core