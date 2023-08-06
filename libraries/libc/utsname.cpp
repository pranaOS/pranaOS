/**
 * @file utsname.cpp
 * @author Krisna Pranav
 * @brief utsname
 * @version 1.0
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <errno.h>
#include <sys/utsname.h>
#include <kernel/api/syscall.h>

extern "C"
{

    /**
     * @param buf 
     * @return int 
     */
    int uname(struct utsname* buf)
    {
        int rc = syscall(SC_uname, buf);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

} // extern