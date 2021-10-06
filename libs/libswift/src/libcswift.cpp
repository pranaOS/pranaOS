/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#include <Math.h>
#include <stdio.h>
#include <sys/types.h>

size_t _swift_stdlib_fwrite_stdout(const void *ptr,
                                                  size_t size,
                                                  size_t nitems) {
    return fwrite(ptr, size, nitems, stdout);
}
