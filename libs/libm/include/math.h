/*
* Copyright (c) 2021, Krisna Pranav
*
* SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <limits.h>
#include <sys/cdefs.h>

#if __cplusplus >= 201103L
#   define NOEXCEPT noexcept
#else
#   define NOEXCEPT
#endif

__BEGIN_DECLS

/* FIXME: define some math funcs + float funcs */

#define MATH_ERRNO 1
#define MATH_ERREXCEPT 2
#define math_errhandling MATH_ERREXCEPT

#define HUGE_VALF __builtin_huge_valf()
#define HUGE_VAL __builtin_huge_val()
#define HUGE_VALL __builtin_huge_vall()
#define INFINITY __builtin_huge_valf()
#define NAN __builtin_nan("")
#define MAXFLOAT FLT_MAX

__END_DECLS