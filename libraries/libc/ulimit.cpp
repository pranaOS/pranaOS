/**
 * @file ulimit.cpp
 * @author Krisna Pranav
 * @brief ulimit
 * @version 6.0
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/format.h>
#include <assert.h>
#include <sys/resource.h>
#include <syscall.h>
#include <ulimit.h>

extern "C" {

/**
 * @param cmd 
 * @param newlimit 
 * @return long 
 */
long ulimit([[maybe_unused]] int cmd, [[maybe_unused]] long newlimit)
{
    dbgln("FIXME: Implement ulimit()");
    TODO();
    return -1;
}

/**
 * @param who 
 * @param usage 
 * @return int 
 */
int getrusage(int who, struct rusage* usage)
{
    int rc = syscall(SC_getrusage, who, usage);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param resource 
 * @param rl 
 * @return int 
 */
int getrlimit([[maybe_unused]] int resource, rlimit* rl)
{
    rl->rlim_cur = RLIM_INFINITY;
    rl->rlim_max = RLIM_INFINITY;
    return 0;
}

/**
 * @param resource 
 * @param rl 
 * @return int 
 */
int setrlimit([[maybe_unused]] int resource, [[maybe_unused]] rlimit const* rl)
{
    return 0;
}
}
