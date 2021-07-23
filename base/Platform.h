/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#ifdef __i386__
#    define BASE_ARCH_I386 1
#endif

#ifdef __x86_64__
#    define BASE_ARCH_X86_64 1
#endif

#if defined(__APPLE__) && defined(__MACH__)
#    define BASE_OS_MACOS
#    define BASE_OS_BSD_GENERIC
#endif
