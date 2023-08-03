/**
 * @file prana.cpp
 * @author Krisna Pranav
 * @brief prana
 * @version 1.0
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav, 2000-2002 Opsycon AB (www.opsycon.se)
 * 
 */

#pragma once 

#include <errno.h>
#include <prana.h>
#include <kernel/api/syscall.h>

extern "C"
{
    /**
     * @param pid 
     * @return int 
     */
    int disown(pid_t pid)
    {
        int rc = syscall(SC_disown, pid);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param path 
     * @param path_length 
     * @return int 
     */
    int module_load(const char* path, size_t path_length)
    {
        int rc = syscall(SC_module_load, path, path_length);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param name 
     * @param name_length 
     * @return int 
     */
    int module_unload(const char* name, size_t name_length)
    {
        int rc = syscall(SC_module_unload, name, name_length);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

} // extern "C"