/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

int kill(pid_t pid, int sig);
int sigaction(int signo, void* callback);
int raise(int sig);

__END_DECLS