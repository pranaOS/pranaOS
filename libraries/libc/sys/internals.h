/**
 * @file internals.h
 * @author Krisna Pranav
 * @brief internals
 * @version 6.0
 * @date 2023-07-31
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <sys/cdefs.h>

__BEGIN_DECLS

typedef void (*AtExitFunction)(void*);

extern void __libc_init();
extern void __malloc_init();
extern void __stdio_init();
extern void _init();
extern bool __environ_is_malloced;
extern bool __stdio_is_initialized;

int __cxa_atexit(AtExitFunction exit_function, void* parameter, void* dso_handle);
void __cxa_finalize(void* dso_handle);
[[noreturn]] void __cxa_pure_virtual() __attribute__((weak));
[[noreturn]] void __stack_chk_fail();

__END_DECLS
