/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <__libc__.h>

__BEGIN_HEADER

#define _JBLEN 6

typedef int jmp_buf[_JBLEN];

__attribute__((noreturn)) void longjmp(jmp_buf j, int r);

__attribute__((returns_twice)) int setjmp(jmp_buf j);

__END_HEADER