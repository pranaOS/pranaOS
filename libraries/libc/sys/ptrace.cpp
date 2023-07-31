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

extern "C" 
{
    
    /**
     * @param request 
     * @param tid 
     * @param addr 
     * @param data 
     * @return int 
     */
    int ptrace(int request, pid_t tid, void* addr, int data)
    {

        u32 out_data;
        Syscall::SC_ptrace_peek_params peek_params;

        if (request == PT_PEEK) {
            peek_params.address = reinterpret_cast<u32*>(addr);
            peek_params.out_data = &out_data;
            addr = &peek_params;
        }

        Syscall::SC_ptrace_params params {
            request,
            tid,
            reinterpret_cast<u8*>(addr),
            data
        };
        
        int rc = syscall(SC_ptrace, &params);

        if (request == PT_PEEK) {
            if (rc < 0) {
                errno = -rc;
                return -1;
            }
            errno = 0;
            return static_cast<int>(out_data);
        }

        __RETURN_WITH_ERRNO(rc, rc, -1);
    }
}