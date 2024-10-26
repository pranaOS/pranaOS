/**
 * @file pranaos.h
 * @author Krisna Pranav
 * @brief pranaOS
 * @version 6.0
 * @date 2024-10-26
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <kernel/api/posix/futex.h>
#include <kernel/api/posix/pranaos.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

__BEGIN_DECLS

/**
 * @return int
 */
int disown(pid_t);

/**
 * @return int
 */
int profiling_enable(pid_t, uint64_t);

/**
 * @return int
 */
int profiling_disable(pid_t);

/**
 * @return int
 */
int profiling_free_buffer(pid_t);

/**
 * @param userspace_address
 * @param futex_op
 * @param value
 * @param timeout
 * @param userspace_address2
 * @param value3
 * @return int
 */
int futex(uint32_t* userspace_address, int futex_op, uint32_t value, const struct timespec* timeout, uint32_t* userspace_address2, uint32_t value3);

/**
 * @param userspace_address
 * @param value
 * @param abstime
 * @param clockid
 * @return ALWAYS_INLINE
 */
static ALWAYS_INLINE int futex_wait(uint32_t* userspace_address, uint32_t value, const struct timespec* abstime, int clockid)
{
    int op;

    if(abstime)
    {
        op = FUTEX_WAIT_BITSET;
        if(clockid == CLOCK_REALTIME || clockid == CLOCK_REALTIME_COARSE)
            op |= FUTEX_CLOCK_REALTIME;
    }
    else
    {
        op = FUTEX_WAIT;
    }
    return futex(userspace_address, op, value, abstime, NULL, FUTEX_BITSET_MATCH_ANY);
}

/**
 * @param userspace_address
 * @param count
 * @return ALWAYS_INLINE
 */
static ALWAYS_INLINE int futex_wake(uint32_t* userspace_address, uint32_t count)
{
    return futex(userspace_address, FUTEX_WAKE, count, NULL, NULL, 0);
}

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
 * @param string
 * @param string_length
 * @return int
 */
int perf_register_string(char const* string, size_t string_length);

/**
 * @brief Get the stack bounds object
 *
 * @param user_stack_base
 * @param user_stack_size
 * @return int
 */
int get_stack_bounds(uintptr_t* user_stack_base, size_t* user_stack_size);

/**
 * @param size
 * @param options
 * @return int
 */
int anon_create(size_t size, int options);

/**
 * @param path
 * @param path_length
 * @param buffer
 * @param buffer_size
 * @return int
 */
int pranaos_readlink(const char* path, size_t path_length, char* buffer, size_t buffer_size);

/**
 * @param name_buffer
 * @param name_buffer_size
 * @param map
 * @param shift_map
 * @param alt_map
 * @param altgr_map
 * @param shift_altgr_map
 * @return int
 */
int getkeymap(char* name_buffer, size_t name_buffer_size, uint32_t* map, uint32_t* shift_map, uint32_t* alt_map, uint32_t* altgr_map, uint32_t* shift_altgr_map);

/**
 * @param name
 * @param map
 * @param shift_map
 * @param alt_map
 * @param altgr_map
 * @param shift_altgr_map
 * @return int
 */
int setkeymap(const char* name, const uint32_t* map, uint32_t* const shift_map, const uint32_t* alt_map, const uint32_t* altgr_map, const uint32_t* shift_altgr_map);

/**
 * @param ptr
 * @param count
 * @return uint16_t
 */
uint16_t internet_checksum(const void* ptr, size_t count);

/**
 * @param command
 * @param arg0
 * @param arg1
 * @return int
 */
int emuctl(uintptr_t command, uintptr_t arg0, uintptr_t arg1);

/**
 * @param path
 * @param path_length
 * @param options
 * @param ...
 * @return int
 */
int pranaos_open(char const* path, size_t path_length, int options, ...);

__END_DECLS
