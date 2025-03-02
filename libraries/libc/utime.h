/**
 * @file utime.h
 * @author Krisna Pranav
 * @brief utime
 * @version 6.0
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <sys/cdefs.h>

__BEGIN_DECLS

/**
 * @param pathname 
 * @return int 
 */
int utime(char const* pathname, const struct utimbuf*);

__END_DECLS
