/**
 * @file internals.h
 * @author Krisna Pranav
 * @brief internals
 * @version 6.0
 * @date 2023-07-31
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <sys/cdefs.h>

__BEGIN_DECLS

typedef void (*AtExitFunction)(void*);

extern void __libc_init(void);
extern void __malloc_init(void);
extern void __stdio_init(void);
extern void __begin_atexit_locking(void);
extern void _init(void);
extern bool __environ_is_malloced;
extern bool __stdio_is_initialized;
extern bool __heap_is_stable;
extern void* __auxiliary_vector;

/**
 * @param exit_function 
 * @param parameter 
 * @param dso_handle 
 * @return int 
 */
int __cxa_atexit(AtExitFunction exit_function, void* parameter, void* dso_handle);

/**
 * @param dso_handle 
 */
void __cxa_finalize(void* dso_handle);
__attribute__((noreturn)) void __cxa_pure_virtual(void) __attribute__((weak));
__attribute__((noreturn)) void __stack_chk_fail(void);
__attribute__((noreturn)) void __stack_chk_fail_local(void);

__END_DECLS
