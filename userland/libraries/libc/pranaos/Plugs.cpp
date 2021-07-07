/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


// includes
#include <stdio.h>
#include <string.h>
#include <libabi/Syscalls.h>
#include <skift/Plugs.h>


#ifndef __KERNEL__

void __plug_assert_failed(const char *expr, const char *file, const char *function, int line)
{
    char buffer[256];
    snprintf(buffer, 256, "Assert failed: %s in %s:%s() ln%d!\n", expr, file, function, line);
    fwrite(buffer, strlen(buffer), 1, stdlog);
    abort();
}

#endif