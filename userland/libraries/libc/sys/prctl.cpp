/**
 * @file prctl.cpp
 * @author Krisna Pranav
 * @brief prctl
 * @version 6.0
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/prctl.h>
#include <syscall.h>

extern "C" {
    /**
     * @param option 
     * @param ... 
     * @return int 
     */
    int prctl(int option, ...)
    {
        va_list args;
        va_start(args, option);

        uintptr_t arg1 = va_arg(args, uintptr_t);
        uintptr_t arg2 = va_arg(args, uintptr_t);

        va_end(args);

        int rc = syscall(SC_prctl, option, arg1, arg2);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }
}
