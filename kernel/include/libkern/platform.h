/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <libkern/c_attrs.h>

ALWAYS_INLINE int ctz32(unsigned int val)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_ctz(val);
#else
    for (u8 i = 0; i < 32; ++i) {
        if ((val >> i) & 1) {
            return i;
        }
    }
    return 0;
#endif
}