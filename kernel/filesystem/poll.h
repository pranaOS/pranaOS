/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <libkernel/list.h>
#include <proc/wait.h>
#include <stdint.h>

#define POLLIN 0x0001
#define POLLPRI 0x0002
#define POLLOUT 0x0004
#define POLLERR 0x0008
#define POLLHUP 0x0010
#define POLLNVAL 0x0020
#define POLLRDNORM 0x0040
#define POLLRDBAND 0x0080
#define POLLWRNORM 0x0100
#define POLLWRBAND 0x0200
#define POLLMSG 0x0400
#define POLLREMOVE 0x1000

struct poll_table {
    struct list_head list;
};

struct poll_entry {
    struct vfs_file *file;
    struct wait_queue_entry wait;
    struct list_head sibling;
};

struct pollfd {
    int32_t fd;
    int16_t events;
    int16_t revents;
};

/**
 * @brief poll_t
 *
 * @param fds
 * @param nfds
 * @return
 */
int poll_t(struct pollfd *fds, uint32_t nfds);

/**
 *
 * @param file
 * @param wu
 * @param ept
 */
void poll_wait(struct vfs_file *file, struct wait_queue_entry *wu, struct poll_table *ept);

/**
 * @brief poll wakeup
 * @param t
 */
void poll_wakeup(struct thread *t);