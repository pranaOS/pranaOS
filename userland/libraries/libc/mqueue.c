/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <errno/error.h>
#include "mqueue.h"
#include <unistd.h>

_syscall3(mq_open, const char *, int, struct mq_attr *);
int 
mq_open(const char *name, int flags, struct mq_attr *attr) {
	SYSCALL_RETURN_ORIGINAL(syscall_meque_open(name, flags, attr));
}