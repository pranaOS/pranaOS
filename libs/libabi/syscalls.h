/*
* Copyright (c) 2021, Krisna Pranav
*
* SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <libc/__libc__.h>

#include <libabi/Result.h>
#include <libutils/Prelude.h>
#include <libabi/Handle.h>
#include <libabi/IOCall.h>
#include <libabi/Launchpad.h>
#include <libabi/System.h>

#ifndef __cplusplus

// FIXME: __cplusplus jresult add more funcs

static inline JResult __syscall(Syscall syscall)
{
    return __syscall(syscall, 0, 0, 0, 0, 0);
}

#endif

__BEGIN_HEADER


JResult J_process_this(int *pid);
JResult J_process_name(char *name, size_t size);
JResult J_process_launch(Launchpad *launchpad, int *pid);
JResult J_process_clone(int *pid, TaskFlags flags);
JResult J_process_exec(Launchpad *launchpad);
JResult J_process_exit(int exit_code);
JResult J_process_cancel(int pid);
JResult J_process_sleep(int time);
JResult J_process_wait(int tid, int *user_exit_value);

JResult J_memory_alloc(size_t size, uintptr_t *out_address);
JResult J_memory_map(uintptr_t address, size_t size, int flags);
JResult J_memory_free(uintptr_t address);
JResult J_memory_include(int handle, uintptr_t *out_address, size_t *out_size);
JResult J_memory_get_handle(uintptr_t address, int *out_handle);

JResult J_filesystem_mkdir(const char *raw_path, size_t size);
JResult J_filesystem_mkpipe(const char *raw_path, size_t size);
JResult J_filesystem_link(const char *raw_old_path, size_t old_size, const char *raw_new_path, size_t new_size);
JResult J_filesystem_unlink(const char *raw_path, size_t size);
JResult J_filesystem_rename(const char *raw_old_path, size_t old_size, const char *raw_new_path, size_t new_size);


// FIXME: system info, handler JResult funcs

__END_HEADER