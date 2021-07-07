/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <pranaos/Printf.h>
#include <stdio.h>
#include <string.h>

void string_printf_append(printf_info_t *info, char c)
{
    if (info->allocated == -1)
    {
        strapd((char *)info->output, c);
    }
    else
    {
        strnapd((char *)info->output, c, info->allocated);
    }
}

int snprintf(char *s, size_t n, const char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);

    int result = vsnprintf(s, n, fmt, va);

    va_end(va);

    return result;
}