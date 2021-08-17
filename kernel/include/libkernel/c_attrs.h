/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef _KERNEL_LIBKERN_C_ATTRS_H
#define _KERNEL_LIBKERN_C_ATTRS_H

#ifndef PACKED
#define PACKED __attribute__((packed))
#endif

#ifndef ALWAYS_INLINE
#define ALWAYS_INLINE __attribute__((always_inline)) inline
#endif

#ifndef NORETURN
#define NORETURN __attribute__((noreturn))
#endif

#endif
