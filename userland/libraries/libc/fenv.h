/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <__libc__.h>
#include <stdint.h>

__BEGIN_HEADER

#define _FENV_H

#define FE_DIVBYZERO 4
#define FE_INEXACT 32
#define FE_INVALID 1
#define FE_OVERFLOW 8
#define FE_UNDERFLOW 16
#define __FE_DENORM 2

#define FE_ALL_EXCEPT 63

#define FE_DOWNWARD 0x400
#define FE_TONEAREST 0
#define FE_TOWARDZERO 0xC00
#define FE_UPWARD 0x800

__END_HEADER