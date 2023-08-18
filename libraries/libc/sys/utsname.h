/**
 * @file utsname.h
 * @author Krisna Pranav
 * @brief utsname
 * @version 6.0
 * @date 2023-07-10
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <sys/cdefs.h>

/// ENTRY_LENGTH
#define UTSNAME_ENTRY_LEN 65

__BEGIN_DECLS

/// @brief utsname
struct utsname {
    char sysname[UTSNAME_ENTRY_LEN];
    char nodename[UTSNAME_ENTRY_LEN];
    char release[UTSNAME_ENTRY_LEN];
    char version[UTSNAME_ENTRY_LEN];
    char machine[UTSNAME_ENTRY_LEN];
};

/**
 * @return int 
 */
int uname(struct utsname*);

__END_DECLS
