/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include "misc.h"
#include <AK/Format.h>

extern "C" {
const char* __cxa_demangle(const char*, void*, void*, int*)
{
    dbgln("WARNING: __cxa_demangle not supported");
    return "";
}

void* __dso_handle __attribute__((__weak__));
}
