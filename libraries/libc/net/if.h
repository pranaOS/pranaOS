/**
 * @file if.h
 * @author Krisna Pranav
 * @brief if
 * @version 6.0
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/api/posix/net/if.h>

__BEGIN_DECLS

/**
 * @param ifname 
 * @return unsigned int 
 */
unsigned int if_nametoindex(char const* ifname);

/**
 * @param ifindex 
 * @param ifname 
 * @return char* 
 */
char* if_indextoname(unsigned int ifindex, char* ifname);

/**
 * @return struct if_nameindex* 
 */
struct if_nameindex* if_nameindex(void);

/**
 * @param ptr 
 */
void if_freenameindex(struct if_nameindex* ptr);

__END_DECLS
