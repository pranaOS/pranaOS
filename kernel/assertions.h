/**
 * @file assertions.h
 * @author Krisna Pranav
 * @brief Assertions
 * @version 6.0
 * @date 2023-06-28
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#define __STRINGIFY_HELPER(x) #x
#define __STRINGIFY(x) __STRINGIFY_HELPER(x)

#ifdef DEBUG

/**
 * @brief assertion_failed
 * 
 * @param msg 
 * @param file 
 * @param line 
 * @param func 
 */
[[noreturn]] void __assertion_failed(const char* msg, const char* file, unsigned line, const char* func);
#    define ASSERT(expr) (static_cast<bool>(expr) ? (void)0 : __assertion_failed(#    expr, __FILE__, __LINE__, __PRETTY_FUNCTION__))
#    define ASSERT_NOT_REACHED() ASSERT(false)
#else
#    define ASSERT(expr)
#    define ASSERT_NOT_REACHED() CRASH()
#endif
#define CRASH()              \
    do {                     \
        asm volatile("ud2"); \
    } while (0)
#define RELEASE_ASSERT(x) \
    do {                  \
        if (!(x))         \
            CRASH();      \
    } while (0)

#define ASSERT_INTERRUPTS_DISABLED() ASSERT(!(cpu_flags() & 0x200))
#define ASSERT_INTERRUPTS_ENABLED() ASSERT(cpu_flags() & 0x200)
#define TODO ASSERT_NOT_REACHED
