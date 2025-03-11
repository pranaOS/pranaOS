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

/**
 * @return ssize_t 
 */
ssize_t lgetxattr(char const*, char const*, void*, size_t)
{
    dbgln("FIXME: Implement lgetxattr()");
    return 0;
}

/**
 * @return ssize_t 
 */
ssize_t fgetxattr(int, char const*, void*, size_t)
{
    dbgln("FIXME: Implement fgetxattr()");
    return 0;
}

/**
 * @return int 
 */
int setxattr(char const*, char const*, void const*, size_t, int)
{
    dbgln("FIXME: Implement setxattr()");
    return 0;
}

/**
 * @return int 
 */
int lsetxattr(char const*, char const*, void const*, size_t, int)
{
    dbgln("FIXME: Implement lsetxattr()");
    return 0;
}

/**
 * @return int 
 */
int fsetxattr(int, char const*, void const*, size_t, int)
{
    dbgln("FIXME: Implement fsetxattr()");
    return 0;
}

/**
 * @return ssize_t 
 */
ssize_t listxattr(char const*, char*, size_t)
{
    dbgln("FIXME: Implement listxattr()");
    return 0;
}

/**
 * @return ssize_t 
 */
ssize_t llistxattr(char const*, char*, size_t)
{
    dbgln("FIXME: Implement llistxattr()");
    return 0;
}

/**
 * @return ssize_t 
 */
ssize_t flistxattr(int, char*, size_t)
{
    dbgln("FIXME: Implement flistxattr()");
    return 0;
}
