/**
 * @file select.cpp
 * @author Krisna Pranav
 * @brief select
 * @version 6.0
 * @date 2023-07-31
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <errno.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>
#include <kernel/api/syscall.h>

extern "C" 
{

    /**
     * @param nfds 
     * @param readfds 
     * @param writefds 
     * @param exceptfds 
     * @param timeout_tv 
     * @return int 
     */
    int select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, timeval* timeout_tv)
    {
        timespec* timeout_ts = nullptr;
        timespec timeout;
        if (timeout_tv) {
            timeout_ts = &timeout;
            TIMEVAL_TO_TIMESPEC(timeout_tv, timeout_ts);
        }
        return pselect(nfds, readfds, writefds, exceptfds, timeout_ts, nullptr);
    } // int select

    /**
     * @param nfds 
     * @param readfds 
     * @param writefds 
     * @param exceptfds 
     * @param timeout 
     * @param sigmask 
     * @return int 
     */
    int pselect(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, const timespec* timeout, const sigset_t* sigmask)
    {
        Syscall::SC_select_params params { nfds, readfds, writefds, exceptfds, timeout, sigmask };
        int rc = syscall(SC_select, &params);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    } // int pselect
    
} // extern
