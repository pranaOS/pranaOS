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
        return (bcd & 0x0F);
    }
}