/**
 * @file ssp.cpp
 * @author Krisna Pranav
 * @brief ssp
 * @version 6.0
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/format.h>
#include <mods/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/internals.h>
#include <unistd.h>

#if defined __SSP__ || defined __SSP_ALL__
#    error "file must not be compiled with stack protection enabled on it. Use -fno-stack-protector"
#endif

extern "C" {

extern size_t __stack_chk_guard;
__attribute__((used)) size_t __stack_chk_guard = (size_t)0xc6c7c8c9;

__attribute__((noreturn)) void __stack_chk_fail()
{
    dbgln("Error: USERSPACE({}) Stack protector failure, stack smashing detected!", getpid());
    if (__stdio_is_initialized)
        warnln("Error: Stack protector failure, stack smashing detected!");
    abort();
}

__attribute__((noreturn)) void __stack_chk_fail_local()
{
    __stack_chk_fail();
}

} // extern "C"
