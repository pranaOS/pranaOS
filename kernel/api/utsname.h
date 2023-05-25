/**
 * @file utsname.h
 * @author Krisna Pranav
 * @brief UTSName
 * @version 0.1
 * @date 2023-05-25
 * 
 * @copyright Copyright (c) 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "types.h"

__DECL_BEGIN

struct utsname {
    char sysname[16];
    char nodename[64];
    char release[16];
    char version[32];
    char machine[16];
};

__DECL_END