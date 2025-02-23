/**
 * @file poll.cpp
 * @author Krisna Pranav
 * @brief POLL
 * @version 6.0
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <errno.h>
#include <poll.h>
#include <sys/time.h>
#include <syscall.h>

extern "C" {

/**
 * @param fds 
 * @param nfds 
 * @param timeout_ms 
 * @return int 
 */
int poll(pollfd* fds, nfds_t nfds, int timeout_ms)
{
    timespec timeout;
    timespec* timeout_ts = &timeout;
    if (timeout_ms < 0)
        timeout_ts = nullptr;
    else
        timeout = { timeout_ms / 1000, (timeout_ms % 1000) * 1'000'000 };
    return ppoll(fds, nfds, timeout_ts, nullptr);
}

/**
 * @param fds 
 * @param nfds 
 * @param timeout 
 * @param sigmask 
 * @return int 
 */
int ppoll(pollfd* fds, nfds_t nfds, timespec const* timeout, sigset_t const* sigmask)
{
    Syscall::SC_poll_params params { fds, nfds, timeout, sigmask };
    int rc = syscall(SC_poll, &params);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}
}
