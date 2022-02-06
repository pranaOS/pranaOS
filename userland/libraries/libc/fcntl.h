/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <sys/types.h>

#define O_ACC_OMDE 0003

#define AT_EACCESS 1
#define AT_SYMLINK_NOFOLLLOW 2
#define AT_SYMLINK_FOLLOW 4

int open(const char* path, int oflag, ...);
int fcntl(int fd, int cmd, ...);
int create(const char* path, mode_t mode);