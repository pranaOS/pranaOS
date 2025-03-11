/**
 * @file fnmatch.h
 * @author Krisna Pranav
 * @brief fn match
 * @version 6.0
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <sys/types.h>

#define FNM_NOMATCH 1
#define FNM_PATHNAME 1
#define FNM_NOESCAPE 2
#define FNM_PERIOD 4
#define FNM_FILE_NAME FNM_PATHNAME
#define FNM_LEADING_DIR 8
#define FNM_CASEFOLD 16
#define FNM_EXTMATCH 32

__BEGIN_DECLS

/**
 * @param pattern 
 * @param string 
 * @param flags 
 * @return int 
 */
int fnmatch(char const* pattern, char const* string, int flags);

__END_DECLS