/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once 

struct mqueue_attr {
    long mqueue_flags;
    long mqueue_maxmsg;
    long mqueue_msgsize;
    long mqueue_curmsgs;
};

int 
mqueue_open(const char *name, int flags, struct mqueue_attr *attr);

int
mqueue_close(int fd);

int 
mqueue_unlink(const char *name);

int 
mqueue_send(int fd, char *buf, unsigned int priority, unsigned int msize);

int
mqueue_receive(int fd, char *buf, unsigned int priorty, unsigned int msize);
