/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libutils/types.h>

#define RREGISTER(num)                         \
    union {                                    \
        u64 r##num;                            \
        struct {                               \
            u32 _unused##num;                  \
            union {                            \
                u32 r##num##d;                 \
                struct {                       \
                    u16 __unused##num;         \
                    union {                    \
                        u16 r##num##w;         \
                        struct {               \
                            u8 ___unused##num; \
                            u8 r##num##b;      \
                        };                     \
                    };                         \
                };                             \
            };                                 \
        };                                     \
    }
