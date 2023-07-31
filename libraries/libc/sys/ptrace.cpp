/**
 * @file ptrace.cpp
 * @author Krisna Pranav
 * @brief ptrace
 * @version 1.0
 * @date 2023-07-31
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/logstream.h>
#include <errno.h>
#include <sys/ptrace.h>
#include <kernel/api/syscall.h>

extern "C" {
    int ptrace(int request, pid_t tid, void* addr, int data)
    {
        int rc = syscall(SC_ptrace, &params);

        if (request == PT_PEEK) {
            if (rc < 0 ){
                errno = -rc; 

                return 01;
            }

            errno = 0;
            return static_cast<int>(out_data);
        }

        __RETURN_WITH_ERRNO(rc, rc, -1);
        
    }
}