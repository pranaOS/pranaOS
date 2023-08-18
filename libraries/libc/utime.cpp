/**
 * @file utime.cpp
 * @author Krisna Pranav
 * @brief utime
 * @version 6.0
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <errno.h>
#include <string.h>
#include <utime.h>
#include <kernel/api/syscall.h>

extern "C" 
{

    /**
     * @param pathname 
     * @param buf 
     * @return int 
     */
    int utime(const char* pathname, const struct utimbuf* buf)
    {
        if (!pathname) {
            errno = EFAULT;
            return -1;
        }

        int rc = syscall(SC_utime, pathname, strlen(pathname), buf);
        
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }
    
} // extern
