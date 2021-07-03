/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void assert_failed(const char *expr, const char *file, const char *function, int line)
{
    printf("Assert Failed: %s %s %s %d\n", expr, file, function, line);
    abort();
}