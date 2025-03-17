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
     * @param data 
     * @return UnixDateTime 
     */
    UnixDateTime time_from_packed_dos(DOSPackedData data)
    {
        if (data.value == 0)
            return UnixDateTime::from_unix_time_parts(first_dos_year);
        
        return data.value;
    }
}