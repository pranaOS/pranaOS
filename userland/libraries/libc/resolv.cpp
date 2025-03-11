/**
 * @file resolv.cpp
 * @author Krisna Pranav
 * @brief resolv
 * @version 6.0
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/format.h>
#include <resolv.h>

extern "C" {

/**
 * @param char 
 * @return int 
 */
int res_query(char const*, int, int, unsigned char*, int)
{
    dbgln("FIXME: Implement res_query()");
    return 0;
}
}
