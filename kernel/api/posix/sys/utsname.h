/**
 * @file utsname.h
 * @author Krisna Pranav
 * @brief utsname
 * @version 6.0
 * @date 2024-09-09
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/api/posix/sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define UTSNAME_ENTRY_LEN 65

struct utsname 
{
    char sysname[UTSNAME_ENTRY_LEN];
    char nodename[UTSNAME_ENTRY_LEN];
    char release[UTSNAME_ENTRY_LEN];
    char version[UTSNAME_ENTRY_LEN];
    char machine[UTSNAME_ENTRY_LEN];
}; // struct utsname

#ifdef __cplusplus
}
#endif
