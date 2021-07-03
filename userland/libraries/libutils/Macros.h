/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#define PACKED __attribute__((packed))

#define ALIGNED(__align) __attribute__((aligned(__align)))

#define UNUSED(__stuff) (void)(__stuff)

#define CREATE(__type) ((__type *)calloc(1, sizeof(__type)))

#define NO_RETURN __attribute__((noreturn))

#define CLEANUP(__function) __attribute__((__cleanup__(__function)))

#define FLATTEN __attribute__((flatten))

#define ALWAYS_INLINE __attribute__((always_inline))