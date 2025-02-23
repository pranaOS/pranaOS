/**
 * @file priority.cpp
 * @author Krisna Pranav
 * @brief priority
 * @version 6.0
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/format.h>
#include <sys/resource.h>

extern "C" {

/**
 * @param which 
 * @param who 
 * @return int 
 */
int getpriority([[maybe_unused]] int which, [[maybe_unused]] id_t who)
{
    dbgln("FIXME: Implement getpriority()");
    return -1;
}

/**
 * @param which 
 * @param who 
 * @param value 
 * @return int 
 */
int setpriority([[maybe_unused]] int which, [[maybe_unused]] id_t who, [[maybe_unused]] int value)
{
    dbgln("FIXME: Implement setpriority()");
    return -1;
}
}
