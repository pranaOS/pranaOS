/**
 * @file dospackedtime.h
 * @author Krisna Pranav
 * @brief DOSPackedTime
 * @version 6.0
 * @date 2025-03-17
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/time.h>
#include <mods/types.h>

namespace Mods 
{

    union DOSPackedTime {
        u16 value;
        struct {
            u16 second : 5;
            u16 minute : 6;
            u16 hour : 5;
        }; // struct
    }; // union DOSPackedTime

    static_assert(sizeof(DOSPackedTime) == 2);

    union DOSPackedDate {
        u16 value;
        struct {
            u16 day : 5;
            u16 month : 4;
            u16 year : 7;
        }; // struct
    }; // union DOSPackedDate

    static_assert(sizeof(DOSPackedDate) == 2);

    inline constexpr u16 first_dos_year = 1980;

    /**
     * @return UnixDateTime 
     */
    UnixDateTime time_from_packed_dos(DOSPackedDate, DOSPackedTime);

    /**
     * @param year 
     * @param month 
     * @param day 
     * @return DOSPackedDate 
     */
    DOSPackedDate to_packed_dos_date(unsigned year, unsigned month, unsigned day);

    /**
     * @param hour 
     * @param minute 
     * @param second 
     * @return DOSPackedTime 
     */
    DOSPackedTime to_packed_dos_time(unsigned hour, unsigned minute, unsigned second);

} // namespace Mods

#if USING_MODS_GLOBALLY
using Mods::DOSPackedDate;
using Mods::DOSPackedTime;
using Mods::time_from_packed_dos;
using Mods::to_packed_dos_date;
using Mods::to_packed_dos_time;
#endif
