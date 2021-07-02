/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <errno.h>
#include <sys/ptrace.h>
#include <syscall.h>

extern "C" {

int ptrace(int request, pid_t tid, void* addr, int data)
{


    u32 out_data;
    Syscall::SC_ptrace_peek_params peek_params;
    auto is_peek_type = request == PT_PEEK || request == PT_PEEKDEBUG;
    if (is_peek_type) {
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

    if (is_peek_type) {
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
