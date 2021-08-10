/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef _KERNEL_LIBKERNEL_BITS_THREAD_H
#define _KERNEL_LIBKERNEL_BITS_THREAD_H

#include <libkernel/types.h>

struct thread_create_params {
    uint32_t entry_point;
    uint32_t stack_start;
    uint32_t stack_size;
};

typedef struct thread_create_params thread_create_params_t;

#endif 