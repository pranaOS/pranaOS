/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <__libc__.h>
#include <stdarg.h>
#include <stddef.h>

__BEGIN_HEADER

typedef struct
{
    int handle;
    int is_eof;
    int error;
} FILE;
#define __DEFINED_FILE

#define BUFSIZ 8192

FILE *__stdio_get_stdin(void);
FILE *__stdio_get_stdout(void);
FILE *__stdio_get_stderr(void);
FILE *__stdio_get_stdlog(void);

__END_HEADER