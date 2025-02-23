/**
 * @file ifaddrs.h
 * @author Krisna Pranav
 * @brief ifaddrs
 * @version 6.0
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/api/posix/ifaddrs.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

/**
 * @param ifap 
 * @return int 
 */
int getifaddrs(struct ifaddrs** ifap);

/**
 * @param ifa 
 */
void freeifaddrs(struct ifaddrs* ifa);

__END_DECLS
