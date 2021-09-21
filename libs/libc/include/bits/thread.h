/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <sys/types.h>

struct thread_create_params {
    uint32_t entry_point;
    uint32_t stack_start;
    uint32_t stack_size;
};

typedef struct thread_create_params thread_create_params_t;
