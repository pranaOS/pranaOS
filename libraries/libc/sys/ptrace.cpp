/**
 * @file ptrace.cpp
 * @author Krisna Pranav
 * @brief ptrace
 * @version 6.0
 * @date 2023-07-31
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <errno.h>
#include <sys/ptrace.h>
#include <syscall.h>

extern "C" {

/**
 * @param request 
 * @param tid 
 * @param addr 
 * @param data 
 * @return long 
 */
long ptrace(int request, pid_t tid, void* addr, void* data)
{
    if (request == PT_PEEKBUF) {
        return EINVAL;
    }

    FlatPtr out_data;

    auto is_peek_type = request == PT_PEEK || request == PT_PEEKDEBUG;

    if (is_peek_type) {
        data = &out_data;
    }

    Syscall::SC_ptrace_params params {
        request,
        tid,
        addr,
        (FlatPtr)data
    };

    long rc = syscall(SC_ptrace, &params);

    if (is_peek_type) {
        if (rc < 0) {
            errno = -rc;
            return -1;
        }
        errno = 0;
        return static_cast<long>(out_data);
    }

    __RETURN_WITH_ERRNO(rc, rc, -1);
}

} // extern "C"
