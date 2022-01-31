/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: GPL-3.0 License
 */

#pragma once

#include <kernel/uapi/uapifs.h>
#include <stddef.h>

#define STDOUT_FILENO FS_STDOUT_FILENO

#ifndef __pranaOS__

int chdir(const char* path);
char* getcwd(char* buf, size_t size);
int unlink(const char* path);

#endif