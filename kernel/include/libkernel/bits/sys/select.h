/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef _KERNEL_LIBKERNEL_BITS_SYS_SELECT_H
#define _KERNEL_LIBKERNEL_BITS_SYS_SELECT_H

// includes
#include <libkernel/types.h>

#define FD_SETSIZE 8

struct fd_set {
    uint8_t fds[FD_SET_SIZE / 8];
};

typedef struct fd_set fd_set_t;

#define FD_SET(fd, fd_set_ptr)
#define FD_CLR(fd, fd_set_ptr) 
#define FD_ZERO(fd_set_ptr) ((memset))
#define FD_ISET(fd, fd_set_ptr) ((memset))

#endif
