/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <sys/cdefs.h>

__BEGIN_DECLS

#ifndef NDEBUG
__attribute__((noreturn)) void __assertion_failed(const char* msg);
#    define __stringify_helper(x) #    x
#    define __stringify(x) __stringify_helper(x)
#    define assert(expr)                                                            \
        (__builtin_expect(!(expr), 0)                                               \
                ? __assertion_failed(#expr "\n" __FILE__ ":" __stringify(__LINE__)) \
                : (void)0)

#else
#    define assert(expr) ((void)(0))
#    define VERIFY_NOT_REACHED() _abort()
#endif

__attribute__((noreturn)) void _abort();

#ifndef __cplusplus
#    define static_assert _Static_assert
#endif

__END_DECLS