/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <libkernel/list.h>
#include <proc/wait.h>
#include <stdint.h>

#define POLLIN 0x0001

struct poll_table {
    struct list_head list;
};  