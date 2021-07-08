/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <__libc__.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>


__BEGIN_HEADER 

extern char **environ;

pid_t getpid(void);
pid_t getppid(void);

int close(int fd);

pid_t fork(void);


__END_HEADER
