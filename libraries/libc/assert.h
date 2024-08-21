/**
 * @file assert.h
 * @author Krisna Pranav
 * @brief assert
 * @version 6.0
 * @date 2023-08-01
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <sys/cdefs.h>

__BEGIN_DECLS

#ifdef DEBUG
__attribute__((noreturn)) void __assertion_failed(const char* msg);
#    define __stringify_helper(x) #    x
#    define __stringify(x) __stringify_helper(x)
#    define assert(expr)                                                           \
        do {                                                                       \
            if (__builtin_expect(!(expr), 0))                                      \
                __assertion_failed(#expr "\n" __FILE__ ":" __stringify(__LINE__)); \
        } while (0)
#    define ASSERT_NOT_REACHED() assert(false)
#else
#    define assert(expr) ((void)0)
#    define ASSERT_NOT_REACHED() CRASH()
#endif

#define CRASH()              \
    do {                     \
        asm volatile("ud2"); \
    } while (0)

/// @brief: ASSERT functionalities.
#define ASSERT assert
#define RELEASE_ASSERT assert
#define TODO ASSERT_NOT_REACHED

__END_DECLS