/**
 * @file syscall.cpp
 * @author Krisna Pranav
 * @brief syscall
 * @version 6.0
 * @date 2024-10-26
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/api/syscall.h>
#include <libsystem/syscall.h>

extern "C" 
{
    /**
     * @param function 
     * @return uintptr_t 
     */
    uintptr_t syscall0(uintptr_t function)
    {
        return Syscall::invoke((Syscall::Function)function);
    }

    /**
     * @param function 
     * @param arg0 
     * @return uintptr_t 
     */
    uintptr_t syscall1(uintptr_t function, uintptr_t arg0)
    {
        return Syscall::invoke((Syscall::Function)function, arg0);
    }

    /**
     * @param function 
     * @param arg0 
     * @param arg1 
     * @return uintptr_t 
     */
    uintptr_t syscall2(uintptr_t function, uintptr_t arg0, uintptr_t arg1)
    {
        return Syscall::invoke((Syscall::Function)function, arg0, arg1);
    }

    /**
     * @param function 
     * @param arg0 
     * @param arg1 
     * @param arg2 
     * @return uintptr_t 
     */
    uintptr_t syscall3(uintptr_t function, uintptr_t arg0, uintptr_t arg1, uintptr_t arg2)
    {
        return Syscall::invoke((Syscall::Function)function, arg0, arg1, arg2);
    }

    /**
     * @param function 
     * @param arg0 
     * @param arg1 
     * @param arg2 
     * @param arg3 
     * @return uintptr_t 
     */
    uintptr_t syscall4(uintptr_t function, uintptr_t arg0, uintptr_t arg1, uintptr_t arg2, uintptr_t arg3)
    {
        return Syscall::invoke((Syscall::Function)function, arg0, arg1, arg2, arg3);
    }
} // extern "C" 