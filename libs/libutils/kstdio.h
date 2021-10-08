/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#ifndef __pranaOS__
#   ifdef KERNEL
#       include <libutils/types.h>
#   else
#   include <libutils/types.h>
#   include <stdarg.h>

extern "C" {

void dbgputstr(const char*, size_t);

}
#   endif
#else
#   include <stdio.h>

inline void dbgputstr(const char* characters, size_t length)
{
    fwrite(characters, 1, length, stderr);
}
#endif