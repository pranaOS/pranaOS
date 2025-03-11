/**
 * @file assertions.h
 * @author Krisna Pranav
 * @brief Assertions
 * @version 6.0
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#if defined(KERNEL)
#    include <kernel/library/assertions.h>
#else
extern "C" __attribute__((noreturn)) void mods_verification_failed(char const*);
#    define __stringify_helper(x) #x
#    define __stringify(x) __stringify_helper(x)
#    define VERIFY(expr)                                                                  \
        (__builtin_expect(!(expr), 0)                                                     \
                ? mods_verification_failed(#expr " at " __FILE__ ":" __stringify(__LINE__)) \
                : (void)0)
#    define VERIFY_NOT_REACHED() VERIFY(false) 
static constexpr bool TODO = false;
#    define TODO() VERIFY(TODO)                
#    define TODO_AARCH64() VERIFY(TODO)        
#    define TODO_RISCV64() VERIFY(TODO)        
#endif
