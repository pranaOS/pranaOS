/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <list.h>

struct __FILE {
    int fd;
    int _flags;
    int _offset;
    int bkup_chr;
    int blksize;
    struct list_head sibling;
};