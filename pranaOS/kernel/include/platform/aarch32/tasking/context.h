/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef _KERNEL_PLATFORM_AARCH32_TASKING_CONTEXT_H
#define _KERNEL_PLATFORM_AARCH32_TASKING_CONTEXT_H


// includes
#include <libkernel/c_attrs.h>
#include <libkernel/types.h>

typedef struct {
    uint32_t r[9];
    uint32_t lr;
} PACKED context_t;

static inline uint32_t context_get_instructions_pointer(context_t* ctx)
{
    return ctx->lr;
}

static inline void context_set_instructions_pointer(context_t* ctx, uint32_t ip)
{
    ctx->lr = ip;
}

#endif