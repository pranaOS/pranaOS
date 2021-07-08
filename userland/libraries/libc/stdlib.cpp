/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <libabi/Syscalls.h>

void abort()
{
    J_process_exit(PROCESS_FAILURE);
    ASSERT_NOT_REACHED();
}

int atoi(const char *str)
{
    int n = 0, neg = 0;

    while (isspace(*str))
    {
        str++;
    }

    switch (*str)
    {
    case '-':
        neg = 1;
        break;
    case '+':
        str++;
        break;
    }

    while (isdigit(*str))
    {
        n = 10 * n - (*str++ - '0');
    }

    return neg ? n : -n;
}