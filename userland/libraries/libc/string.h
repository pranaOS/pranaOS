/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <__libc__.h>
#include <stddef.h>
#include <stdint.h>

__BEGIN_HEADERS

void *memset(void *dest, int c, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
void memmove(void *dest, const void *src, size_t n);

void *memchr(const void *src, int c, size_t n);
void *memrchr(const void *m, int c, size_t n);
int memcmp(const void *v1, const void *vr, size_t n);


__END_HEADERS