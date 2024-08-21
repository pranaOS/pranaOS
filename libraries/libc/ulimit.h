/**
 * @file ulimit.h
 * @author Krisna Pranav
 * @brief ulimit
 * @version 6.0
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <sys/cdefs.h>

__BEGIN_DECLS

/**
 * @param cmd 
 * @param newlimit 
 * @return long 
 */
long ulimit(int cmd, long newlimit);

__END_DECLS
