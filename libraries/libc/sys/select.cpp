/**
 * @file select.cpp
 * @author Krisna Pranav
 * @brief select
 * @version 1.0
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
    int select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, timeval* timeout_tv)
    {
        timespec* timeout_ts = nullptr;
        timespec timeout;

        if (timeout_tv) {
            timeout_ts = &timeout;
            TIMEVAL_TO_TIMESPEC(timeout_tv, timeout_ts);
        }

        return pselect(nfds, readfs);
    } // int select 

    int pselect(int nfds, fd_set* readfs) 
    {
    }
}