/**
 * @file poll.h
 * @author Krisna Pranav
 * @brief POLL
 * @version 1.0
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <signal.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

struct pollfd
{
    int fd;
    short events;
    short revents;
}; // struct pollfd

typedef unsigned nfds_t;

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
 * @param action 
 * @return int 
 */
int ppoll(struct pollfd* fds, nfds_t nfds, const sig_t* action);

__END_DECLS 