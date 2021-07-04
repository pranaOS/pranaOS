/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <string.h>
#include "archs/x86/CPUID.h"

static inline int cpuid_string(int code, int where[4])
{
    asm volatile("cpuid"
                 : "=a"(*where), "=b"(*(where + 0)),
                   "=d"(*(where + 1)), "=c"(*(where + 2))
                 : "a"(code));
    return (int)where[0];
}
