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

#define SYSCALL_LIST(__ENTRY)     \
    __ENTRY(J_PROCESS_THIS)      \
    __ENTRY(J_PROCESS_NAME)      \
    __ENTRY(J_PROCESS_LAUNCH)    \
    __ENTRY(J_PROCESS_CLONE)     \
    __ENTRY(J_PROCESS_EXEC)      \
    __ENTRY(J_PROCESS_EXIT)      \
    __ENTRY(J_PROCESS_CANCEL)    \
    __ENTRY(J_PROCESS_SLEEP)     \
    __ENTRY(J_PROCESS_WAIT)      \
    __ENTRY(J_MEMORY_ALLOC)      \
    __ENTRY(J_MEMORY_MAP)        \
    __ENTRY(J_MEMORY_FREE)       \
    __ENTRY(J_MEMORY_INCLUDE)    \
    __ENTRY(J_MEMORY_GET_HANDLE) \
    __ENTRY(J_FILESYSTEM_LINK)   \
    __ENTRY(J_FILESYSTEM_UNLINK) \
    __ENTRY(J_FILESYSTEM_RENAME) \
    __ENTRY(J_FILESYSTEM_MKPIPE) \
    __ENTRY(J_FILESYSTEM_MKDIR)  \
    __ENTRY(J_SYSTEM_INFO)       \
    __ENTRY(J_SYSTEM_STATUS)     \
    __ENTRY(J_SYSTEM_TIME)       \
    __ENTRY(J_SYSTEM_TICKS)      \
    __ENTRY(J_SYSTEM_REBOOT)     \
    __ENTRY(J_SYSTEM_SHUTDOWN)   \
    __ENTRY(J_HANDLE_OPEN)       \
    __ENTRY(J_HANDLE_CLOSE)      \
    __ENTRY(J_HANDLE_REOPEN)     \
    __ENTRY(J_HANDLE_COPY)       \
    __ENTRY(J_HANDLE_POLL)       \
    __ENTRY(J_HANDLE_READ)       \
    __ENTRY(J_HANDLE_WRITE)      \
    __ENTRY(J_HANDLE_CALL)       \
    __ENTRY(J_HANDLE_SEEK)       \
    __ENTRY(J_HANDLE_STAT)       \
    __ENTRY(J_HANDLE_CONNECT)    \
    __ENTRY(J_HANDLE_ACCEPT)     \
    __ENTRY(J_CREATE_PIPE)       \
    __ENTRY(J_CREATE_TERM)

#define SYSCALL_ENUM_ENTRY(__entry) __entry,

enum Syscall
{
    SYSCALL_LIST(SYSCALL_ENUM_ENTRY) __SYSCALL_COUNT
};

static JResult __syscall(Syscall syscall, uintptr_t p1, uintptr_t p2, uintptr_t p3, uintptr_t p4, uintptr_t p5)
{
    JResult __ret = ERR_NOT_IMPLEMENTED;

#if defined(__x86_64__)

    asm volatile("push %%rbx; movq %2,%%rbx; push %%rbp; int $0x80; pop %%rbp; pop %%rbx"
                 : "=a"(__ret)
                 : "a"(syscall), "r"(p1), "c"(p2), "d"(p3), "S"(p4), "D"(p5)
                 : "memory");

#elif defined(__i386__)
    // ebp need to be saved because J_process_clone will trash it.
    asm volatile("push %%ebx; movl %2,%%ebx; push %%ebp; int $0x80; pop %%ebp; pop %%ebx"
                 : "=a"(__ret)
                 : "0"(syscall), "r"(p1), "c"(p2), "d"(p3), "S"(p4), "D"(p5)
                 : "memory");
#endif

    return __ret;
}

#ifdef __cplusplus

static inline JResult __syscall(Syscall syscall, uintptr_t p1, uintptr_t p2, uintptr_t p3, uintptr_t p4)
{
    return __syscall(syscall, p1, p2, p3, p4, 0);
}

static inline JResult __syscall(Syscall syscall, uintptr_t p1, uintptr_t p2, uintptr_t p3)
{
    return __syscall(syscall, p1, p2, p3, 0, 0);
}

static inline JResult __syscall(Syscall syscall, uintptr_t p1, uintptr_t p2)
{
    return __syscall(syscall, p1, p2, 0, 0, 0);
}

static inline JResult __syscall(Syscall syscall, uintptr_t p1)
{
    return __syscall(syscall, p1, 0, 0, 0, 0);
}

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

JResult J_system_info(SystemInfo *info);
JResult J_system_status(SystemStatus *status);
JResult J_system_time(TimeStamp *timestamp);
JResult J_system_tick(uint32_t *tick);
JResult J_system_reboot();
JResult J_system_shutdown();

JResult J_create_pipe(int *reader_handle, int *writer_handle);
JResult J_create_term(int *server_handle, int *client_handle);

JResult J_handle_open(int *handle, const char *raw_path, size_t size, JOpenFlag flags);
JResult J_handle_close(int handle);
JResult J_handle_reopen(int handle, int *reopened);
JResult J_handle_copy(int source, int destination);
JResult J_handle_poll(HandlePoll *handles, size_t count, Timeout timeout);
JResult J_handle_read(int handle, void *buffer, size_t size, size_t *read);
JResult J_handle_write(int handle, const void *buffer, size_t size, size_t *written);
JResult J_handle_call(int handle, IOCall request, void *args);
JResult J_handle_seek(int handle, ssize64_t *offset, JWhence whence, ssize64_t *result);
JResult J_handle_stat(int handle, JStat *state);
JResult J_handle_connect(int *handle, const char *raw_path, size_t size);
JResult J_handle_accept(int handle, int *connection_handle);

__END_HEADER