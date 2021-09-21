/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <bits/fcntl.h>
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

int open(const char* pathname, int flags);
int creat(const char* path, mode_t mode);

__END_DECLS
