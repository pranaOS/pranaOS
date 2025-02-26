    /**
 * @file account.cpp
 * @author Krisna Pranav
 * @brief account
 * @version 6.0
 * @date 2025-02-26
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/base64.h>
#include <mods/memory.h>
#include <mods/random.h>
#include <mods/scopeguard.h>
#include <libcore/account.h>
#include <libcore/system.h>
#include <libcore/umaskscope.h>
#include <errno.h>
#include <grp.h>
#include <pwd.h>
#ifndef MODS_OS_BSD_GENERIC
#    include <crypt.h>
#    include <shadow.h>
#endif
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

namespace Core 
{
    
} // namespace Core 