/**
 * @file prana.h
 * @author Krisna Pranav
 * @brief prana LibC
 * @version 6.0
 * @date 2023-07-25
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <stdio.h>
#include <unistd.h>

__BEGIN_DECLS


#define THREAD_PRIORITY_MIN 1
#define THREAD_PRIORITY_LOW 10
#define THREAD_PRIORITY_NORMAL 30
#define THREAD_PRIORITY_HIGH 50
#define THREAD_PRIORITY_MAX 99


#define FUTEX_WAIT 1
#define FUTEX_WAKE 2

#define PERF_EVENT_MALLOC 1
#define PERF_EVENT_FREE 2


#define PURGE_ALL_VOLATILE 0x1
#define PURGE_ALL_CLEAN_INODE 0x2

/**
 * @return int 
 */
int disown(pid_t);

/**
 * @brief create
 * 
 * @param buffer 
 * @return int 
 */
int shbuf_create(int, void** buffer);

/**
 * @brief allow_pid
 * 
 * @param peer_pid 
 * @return int 
 */
int shbuf_allow_pid(int, pid_t peer_pid);

/**
 * @return int 
 */
int shbuf_allow_all(int);

/**
 * @param shbuf_id 
 * @param size 
 * @return void* 
 */
void* shbuf_get(int shbuf_id, size_t* size);

/**
 * @param shbuf_id 
 * @return int 
 */
int shbuf_release(int shbuf_id);

/**
 * @param shbuf_id 
 * @return int 
 */
int shbuf_seal(int shbuf_id);

/**
 * @param path 
 * @param path_length 
 * @return int 
 */
int module_load(const char* path, size_t path_length);

/**
 * @param name 
 * @param name_length 
 * @return int 
 */
int module_unload(const char* name, size_t name_length);

/**
 * @return int 
 */
int profiling_enable(pid_t);

/**
 * @return int 
 */
int profiling_disable(pid_t);

/**
 * @param tid 
 * @param amount 
 * @return int 
 */
int set_thread_boost(pid_t tid, int amount);

/**
 * @param amount 
 * @return int 
 */
int set_process_boost(pid_t, int amount);

/**
 * @param userspace_address 
 * @param futex_op 
 * @param value 
 * @param timeout 
 * @return int 
 */
int futex(int32_t* userspace_address, int futex_op, int32_t value, const struct timespec* timeout);

/**
 * @param mode 
 * @return int 
 */
int purge(int mode);

/**
 * @param type 
 * @param arg1 
 * @param arg2 
 * @return int 
 */
int perf_event(int type, uintptr_t arg1, uintptr_t arg2);

/**
 * @param user_stack_base 
 * @param user_stack_size 
 * @return int 
 */
int get_stack_bounds(uintptr_t* user_stack_base, size_t* user_stack_size);

#ifdef __i386__
ALWAYS_INLINE void send_secret_data_to_userspace_emulator(uintptr_t data1, uintptr_t data2, uintptr_t data3)
{
    asm volatile(
        ".byte 0xd6\n"
        ".byte 0xd6\n" ::
            : "eax");
    asm volatile(
        "push %%eax\n"
        "push %%ecx\n"
        "push %%edx\n"
        "pop %%edx\n"
        "pop %%ecx\n"
        "pop %%eax\n" ::"a"(data1),
        "c"(data2), "d"(data3)
        : "memory");
}
#elif __x86_64__
ALWAYS_INLINE void send_secret_data_to_userspace_emulator(uintptr_t, uintptr_t, uintptr_t)
{
}
#endif

__END_DECLS
