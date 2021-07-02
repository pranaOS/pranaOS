/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <fd_set.h>
#include <signal.h>
#include <string.h>
#include <sys/cdefs.h>
#include <sys/time.h>
#include <sys/types.h>

__BEGIN_DECLS

int select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, struct timeval* timeout);
int pselect(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, const struct timespec* timeout, const sigset_t* sigmask);

__END_DECLS