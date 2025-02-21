/**
 * @file xattr.cpp
 * @author Krisna Pranav
 * @brief xattr
 * @version 6.0
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */


#include <mods/format.h>
#include <sys/xattr.h>

/**
 * @return ssize_t 
 */
ssize_t getxattr(char const*, char const*, void*, size_t)
{
    dbgln("FIXME: Implement getxattr()");
    return 0;
}
