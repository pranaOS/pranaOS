/**
 * @file assert.h
 * @author Krisna Pranav
 * @brief assert
 * @version 6.0
 * @date 2023-08-01
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#ifndef _ASSERT_H
#    define _ASSERT_H

#    define __stringify_helper(x) #    x
#    define __stringify(x) __stringify_helper(x)

#    ifndef __cplusplus
#        define static_assert _Static_assert
#    endif
#endif

#include <sys/cdefs.h>

#undef assert

__BEGIN_DECLS

#ifndef NDEBUG
__attribute__((noreturn)) void __assertion_failed(char const* msg);
#    define assert(expr)                                                            \
        (__builtin_expect(!(expr), 0)                                               \
                ? __assertion_failed(#expr "\n" __FILE__ ":" __stringify(__LINE__)) \
                : (void)0)

#else
#    define assert(expr) ((void)(0))
#endif

__END_DECLS
