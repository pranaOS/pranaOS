/**
 * @file ulimit.cpp
 * @author Krisna Pranav
 * @brief ulimit
 * @version 1.0
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <assert.h>
#include <ulimit.h>
#include <sys/resource.h>
#include <mods/logstream.h>

extern "C" 
{

    /**
     * @param cmd 
     * @param newlimit 
     * @return long 
     */
    long ulimit(int cmd, long newlimit)
    {
        (void)cmd;
        (void)newlimit;

        ASSERT_NOT_REACHED();

        return -1;
    }

    /**
     * @param who 
     * @param usage 
     * @return int 
     */
    int getrusage(int who, struct rusage* usage)
    {
        (void)who;
        (void)usage;

        dbg() << "[libc]: getrusage is not implemented yet.. ;(";

        return -1;
    }

} // extern
