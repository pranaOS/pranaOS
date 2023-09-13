/**
 * @file rtc.cpp
 * @author Krisna Pranav
 * @brief rtc 
 * @version 6.0
 * @date 2023-09-13
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/assertions.h>
#include <mods/logstream.h>
#include <mods/time.h>
#include <kernel/cmos.h>
#include <kernel/rtc.h>

namespace RTC 
{
    static time_t s_boot_time;

    void initialize()
    {
        s_boot_time = now();
    }

    /**
     * @return time_t 
     */
    time_t boot_time()
    {
        return s_boot_time;
    }

    /**
     * @return true 
     * @return false 
     */
    static bool update_in_progress()
    {
        return CMOS::read(0x0a) & 0x80;
    }

    /**
     * @param bcd 
     * @return u8 
     */
    static u8 bcd_to_binary(u8 bcd)
    {
        return (bcd & 0x0F) + ((bcd >> 4) * 10);
    }

    /**
     * @param year 
     * @param month 
     * @param day 
     * @param hour 
     * @param minute 
     * @param second 
     */
    void read_registers(unsigned& year, unsigned& month, unsigned& day, unsigned& hour, unsigned& minute, unsigned& second)
    {
        while (update_in_progress())
            ;

        u8 status_b = CMOS::read(0x0b);

        second = CMOS::read(0x00);
        minute = CMOS::read(0x02);
        hour = CMOS::read(0x04);
        day = CMOS::read(0x07);
        month = CMOS::read(0x08);
        year = CMOS::read(0x09);

        bool is_pm = hour & 0x80;

        if (!(status_b & 0x04)) {
            second = bcd_to_binary(second);
            minute = bcd_to_binary(minute);
            hour = bcd_to_binary(hour & 0x7F);
            day = bcd_to_binary(day);
            month = bcd_to_binary(month);
            year = bcd_to_binary(year);
        }

        if (!(status_b & 0x02)) {
            hour %= 12;
            if (is_pm)
                hour += 12;
        }

        year += 2000;
    }
    
    /**
     * @return time_t 
     */
    time_t now()
    {
        while (update_in_progress())
            ;

        unsigned year, month, day, hour, minute, second;
        read_registers(year, month, day, hour, minute, second);

        klog() << "RTC: Year: " << year << ", month: " << month << ", day: " << day << ", hour: " << hour << ", minute: " << minute << ", second: " << second;

        ASSERT(year >= 2018);

        time_t days_since_epoch = years_to_days_since_epoch(year) + day_of_year(year, month, day);
        return ((days_since_epoch * 24 + hour) * 60 + minute) * 60 + second;
    }

} // namespace RTC