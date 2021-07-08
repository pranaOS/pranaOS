/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// incldues
#include <libutils/Prelude.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN(__x, __y) ((__x) < (__y) ? (__x) : (__y))


void *memchr(const void *str, int c, size_t n)
{
    unsigned char *s = (unsigned char *)str;

    for (size_t i = 0; i < n; i++)
    {
        if (*(s + i) == c)
        {
            return (s + i);
        }
    }

    return nullptr;
}