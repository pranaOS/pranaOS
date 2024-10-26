/**
 * @file syscall.h
 * @author Krisna Pranav
 * @brief syscall
 * @version 6.0
 * @date 2024-10-26
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/api/syscall.h>
#include <sys/types.h>

extern "C" 
{
    /**
     * @param function 
     * @return uintptr_t 
     */
    uintptr_t syscall0(uintptr_t function);

    /**
     * @param function 
     * @param arg0 
     * @return uintptr_t 
     */
    uintptr_t syscall1(uintptr_t function, uintptr_t arg0);

    /**
     * @param function 
     * @param arg0 
     * @param arg1 
     * @return uintptr_t 
     */
    uintptr_t syscall2(uintptr_t function, uintptr_t arg0, uintptr_t arg1);

    /**
     * @param function 
     * @param arg0 
     * @param arg1 
     * @param arg2 
     * @return uintptr_t 
     */
    uintptr_t syscall3(uintptr_t function, uintptr_t arg0, uintptr_t arg1, uintptr_t arg2);

    /**
     * @param function 
     * @param arg0 
     * @param arg1 
     * @param arg2 
     * @param arg3 
     * @return uintptr_t 
     */
    uintptr_t syscall4(uintptr_t function, uintptr_t arg0, uintptr_t arg1, uintptr_t arg2, uintptr_t arg3);
}

#ifdef __cplusplus

/**
 * @param function 
 * @return uintptr_t 
 */
inline uintptr_t syscall(auto function)
{
    return syscall0(function);
}

/**
 * @param function 
 * @param arg0 
 * @return uintptr_t 
 */
inline uintptr_t syscall(auto function, auto arg0)
{
    return syscall1((uintptr_t)function, (uintptr_t)arg0);
}

/**
 * @param function 
 * @param arg0 
 * @param arg1 
 * @return uintptr_t 
 */
inline uintptr_t syscall(auto function, auto arg0, auto arg1)
{
    return syscall2((uintptr_t)function, (uintptr_t)arg0, (uintptr_t)arg1);
}

/**
 * @param function 
 * @param arg0 
 * @param arg1 
 * @param arg2 
 * @return uintptr_t 
 */
inline uintptr_t syscall(auto function, auto arg0, auto arg1, auto arg2)
{
    return syscall3((uintptr_t)function, (uintptr_t)arg0, (uintptr_t)arg1, (uintptr_t)arg2);
}

/**
 * @param function 
 * @param arg0 
 * @param arg1 
 * @param arg2 
 * @param arg3 
 * @return uintptr_t 
 */
inline uintptr_t syscall(auto function, auto arg0, auto arg1, auto arg2, auto arg3)
{
    return syscall4((uintptr_t)function, (uintptr_t)arg0, (uintptr_t)arg1, (uintptr_t)arg2, (uintptr_t)arg3);
}

#endif
