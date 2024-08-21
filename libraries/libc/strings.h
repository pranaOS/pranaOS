/**
 * @file strings.h
 * @author Krisna Pranav
 * @brief strings
 * @version 6.0
 * @date 2023-08-04
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

/**
 * @return int 
 */
int strcasecmp(const char*, const char*);

/**
 * @return int 
 */
int strncasecmp(const char*, const char*, size_t);

__END_DECLS
