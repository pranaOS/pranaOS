/**
 * @file poll.h
 * @author Krisna Pranav
 * @brief POLL
 * @version 6.0
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/api/posix/poll.h>
#include <signal.h>

__BEGIN_DECLS

/**
 * @param fds 
 * @param nfds 
 * @param timeout 
 * @return int 
 */
int poll(struct pollfd* fds, nfds_t nfds, int timeout);

/**
 * @param fds 
 * @param nfds 
 * @param timeout 
 * @param sigmask 
 * @return int 
 */
int ppoll(struct pollfd* fds, nfds_t nfds, const struct timespec* timeout, sigset_t const* sigmask);

__END_DECLS
