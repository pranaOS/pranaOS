/**
 * @file utime.h
 * @author Krisna Pranav
 * @brief utime
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
 * @param pathname 
 * @return int 
 */
int utime(const char* pathname, const struct utimbuf*);

__END_DECLS