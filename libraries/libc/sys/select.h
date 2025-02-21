/**
 * @file select.h
 * @author Krisna Pranav
 * @brief select
 * @version 6.0
 * @date 2023-07-31
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <fd_set.h>
#include <signal.h>
#include <string.h>
#include <sys/cdefs.h>
#include <sys/time.h>
#include <sys/types.h>

__BEGIN_DECLS

/**
 * @param nfds 
 * @param readfds 
 * @param writefds 
 * @param exceptfds 
 * @param timeout 
 * @return int 
 */
int select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, struct timeval* timeout);

/**
 * @param nfds 
 * @param readfds 
 * @param writefds 
 * @param exceptfds 
 * @param timeout 
 * @param sigmask 
 * @return int 
 */
int pselect(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, const struct timespec* timeout, sigset_t const* sigmask);

__END_DECLS
