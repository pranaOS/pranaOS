/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/Format.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/internals.h>
#include <syscall.h>
#include <unistd.h>

extern "C" {

extern bool __stdio_is_initialized;
#ifndef NDEBUG
void __asertion_failed(const char *msg)
{
    dbgln("ASSERTION FAILED: {}", msg);

    abort()
}
#endif

}

void _abort()
{
    asm volatile("ud2");
    builtin_unreachable();
}