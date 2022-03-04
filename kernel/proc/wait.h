/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <stdint.h>
#include <utilities/list.h>
#include <libc/sys/types.h>

/**
 * @brief infop
 * 
 */
struct infop {
    pid_t si_pid;
    int32_t si_signo;
    int32_t si_status;
    int32_t si_code;
};

struct thread;

/**
 * @brief wait queue
 * 
 */
typedef void (*wait_queue_func)(struct thread *);

/**
 * @brief queue head
 * 
 */
struct wait_queue_head {
    struct list_head list;
};