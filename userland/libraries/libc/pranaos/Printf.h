/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libutils/Prelude.h>

struct printf_info;

typedef void (*printf_append_t)(struct printf_info *info, char c);

typedef enum
{
    PFSTATE_ESC,
    PFSTATE_PARSE,
    PFSTATE_FORMAT_LENGTH,
    PFSTATE_FINALIZE
} printf_state_t;

typedef enum
{
    PFALIGN_LEFT,
    PFALIGN_RIGHT
} printf_align_t;


typedef struct printf_info
{
    char c;
    printf_state_t state;
    printf_append_t append;

    const char *format;
    int format_offset;

    char padding;
    printf_align_t align;
    size_t lenght;

    void *output;
    int written;
    int allocated;
} printf_info_t;

typedef int (*printf_formatter_impl_t)(printf_info_t *info, va_list va);