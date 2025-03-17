/**
 * @file dospackedtime.cpp
 * @author Krisna Pranav
 * @brief dospacked time
 * @version 6.0
 * @date 2025-03-17
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/dospackedtime.h>

namespace Mods 
{

    /**
     * @param date 
     * @param time 
     * @return UnixDateTime 
     */
    UnixDateTime time_from_packed_dos(DOSPackedDate date, DOSPackedTime time)
    {
        if (date.value == 0)
            return UnixDateTime::from_unix_time_parts(first_dos_year, 1, 1, 0, 0, 0, 0);

        return UnixDateTime::from_unix_time_parts(first_dos_year + date.year, date.month, date.day, time.hour, time.minute, time.second * 2, 0);
    }

    /**
     * @param year 
     * @param month 
     * @param day 
     * @return DOSPackedDate 
     */
    DOSPackedDate to_packed_dos_date(unsigned year, unsigned month, unsigned day)
    {
        DOSPackedDate date;
        date.year = year - first_dos_year;
        date.month = month;
        date.day = day;

        return date;
    }

    /**
     * @param hour 
     * @param minute 
     * @param second 
     * @return DOSPackedTime 
     */
    DOSPackedTime to_packed_dos_time(unsigned hour, unsigned minute, unsigned second)
    {
        DOSPackedTime time;
        time.hour = hour;
        time.minute = minute;
        time.second = second / 2;

        return time;
    }

} // namespace Mods
