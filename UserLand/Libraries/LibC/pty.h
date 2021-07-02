/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <sys/cdefs.h>
#include <termios.h>

__BEGIN_DECLS

int openpty(int* amaster, int* aslave, char* name, const struct termios* termp, const struct winsize* winp);
pid_t forkpty(int* amaster, char* name, const struct termios* termp, const struct winsize* winp);
int login_tty(int fd);

__END_DECLS
