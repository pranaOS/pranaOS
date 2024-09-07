/**
 * @file poll.h
 * @author Krisna Pranav
 * @brief poll
 * @version 6.0
 * @date 2024-09-07
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/api/posix/sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define POLLIN (1u << 0)
#define POLLRDNORM POLLIN
#define POLLPRI (1u << 1)
#define POLLOUT (1u << 2)
#define POLLWRNORM POLLOUT
#define POLLERR (1u << 3)
#define POLLHUP (1u << 4)
#define POLLNVAL (1u << 5)
#define POLLWRBAND (1u << 12)
#define POLLRDHUP (1u << 13)

struct pollfd 
{
    int fd;
    short events;
    short revents;
};

typedef unsigned nfds_t;

#ifdef __cplusplus
}
#endif
