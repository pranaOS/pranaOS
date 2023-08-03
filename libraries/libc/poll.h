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

/// @brief: POLL[IN, PRI, OUT, ERR, HUP, NVAL, RDHUP]
#define POLLIN (1u << 0)
#define POLLPRI (1u << 1)
#define POLLOUT (1u << 2)
#define POLLERR (1u << 3)
#define POLLHUP (1u << 4)
#define POLLNVAL (1u << 5)
#define POLLRDHUP (1u << 13)

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
 * @param timeout 
 * @param sigmask 
 * @return int 
 */
int ppoll(struct pollfd* fds, nfds_t nfds, const struct timespec* timeout, const sigset_t* sigmask);

__END_DECLS