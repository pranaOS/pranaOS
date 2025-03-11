/**
 * @file ifaddrs.cpp
 * @author Krisna Pranav
 * @brief ifaddrs
 * @version 6.0
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <libc/errno.h>
#include <libc/ifaddrs.h>

/**
 * @return int 
 */
int getifaddrs(struct ifaddrs**)
{
    errno = ENOSYS;
    return -1;
}

void freeifaddrs(struct ifaddrs*)
{
}
