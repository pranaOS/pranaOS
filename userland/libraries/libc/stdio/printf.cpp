/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


// includes
#include <pranaos/Printf.h>
#include <stdio.h>
#include <string.h>

void file_append(printf_info_t *info, char c)
{
    fputc(c, (FILE *)info->output);
}

int vfprintf(FILE *file, const char *fmt, va_list va)
{
    printf_info_t info = {};

    info.format = fmt;
    info.append = file_append;
    info.output = file;
    info.allocated = -1;

    return __printf(&info, va);
}

int vprintf(const char *fmt, va_list va)
{
    return vfprintf(stdout, fmt, va);
}