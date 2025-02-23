/**
 * @file pranaos.cpp
 * @author Krisna Pranav
 * @brief pranaos
 * @version 6.0
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/api/posix/fcntl.h>
#include <kernel/api/syscall.h>
#include <arpa/inet.h>
#include <errno.h>
#include <pranaos.h>
#include <string.h>
#include <syscall.h>

extern "C" {

/**
 * @param pid 
 * @return int 
 */
int disown(pid_t pid)
{
    int rc = syscall(SC_disown, pid);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param pid 
 * @param event_mask 
 * @return int 
 */
int profiling_enable(pid_t pid, uint64_t event_mask)
{
    int rc = syscall(SC_profiling_enable, pid, &event_mask);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param pid 
 * @return int 
 */
int profiling_disable(pid_t pid)
{
    int rc = syscall(SC_profiling_disable, pid);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param pid 
 * @return int 
 */
int profiling_free_buffer(pid_t pid)
{
    int rc = syscall(SC_profiling_free_buffer, pid);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param userspace_address 
 * @param futex_op 
 * @param value 
 * @param timeout 
 * @param userspace_address2 
 * @param value3 
 * @return int 
 */
int futex(uint32_t* userspace_address, int futex_op, uint32_t value, const struct timespec* timeout, uint32_t* userspace_address2, uint32_t value3)
{
    int rc;
    switch (futex_op & FUTEX_CMD_MASK) {
    case FUTEX_WAKE_OP: {
        Syscall::SC_futex_params params {
            .userspace_address = userspace_address,
            .futex_op = futex_op,
            .val = value,
            .val2 = (FlatPtr)timeout,
            .userspace_address2 = userspace_address2,
            .val3 = value3
        };
        rc = syscall(SC_futex, &params);
        break;
    }
    default: {
        Syscall::SC_futex_params params {
            .userspace_address = userspace_address,
            .futex_op = futex_op,
            .val = value,
            .timeout = timeout,
            .userspace_address2 = userspace_address2,
            .val3 = value3
        };
        rc = syscall(SC_futex, &params);
        break;
    }
    }
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param mode 
 * @return int 
 */
int purge(int mode)
{
    int rc = syscall(SC_purge, mode);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param type 
 * @param arg1 
 * @param arg2 
 * @return int 
 */
int perf_event(int type, uintptr_t arg1, FlatPtr arg2)
{
    int rc = syscall(SC_perf_event, type, arg1, arg2);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param string 
 * @param string_length 
 * @return int 
 */
int perf_register_string(char const* string, size_t string_length)
{
    int rc = syscall(SC_perf_register_string, string, string_length);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @brief Get the stack bounds object
 * 
 * @param user_stack_base 
 * @param user_stack_size 
 * @return int 
 */
int get_stack_bounds(uintptr_t* user_stack_base, size_t* user_stack_size)
{
    int rc = syscall(SC_get_stack_bounds, user_stack_base, user_stack_size);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param size 
 * @param options 
 * @return int 
 */
int anon_create(size_t size, int options)
{
    int rc = syscall(SC_anon_create, size, options);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param path 
 * @param path_length 
 * @param buffer 
 * @param buffer_size 
 * @return int 
 */
int pranaos_readlink(char const* path, size_t path_length, char* buffer, size_t buffer_size)
{
    Syscall::SC_readlink_params small_params {
        { path, path_length },
        { buffer, buffer_size }
    };
    int rc = syscall(SC_readlink, &small_params);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param name 
 * @param map 
 * @param shift_map 
 * @param alt_map 
 * @param altgr_map 
 * @param shift_altgr_map 
 * @return int 
 */
int setkeymap(char const* name, u32 const* map, u32* const shift_map, u32 const* alt_map, u32 const* altgr_map, u32 const* shift_altgr_map)
{
    Syscall::SC_setkeymap_params params { map, shift_map, alt_map, altgr_map, shift_altgr_map, { name, strlen(name) } };
    return syscall(SC_setkeymap, &params);
}

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
int getkeymap(char* name_buffer, size_t name_buffer_size, u32* map, u32* shift_map, u32* alt_map, u32* altgr_map, u32* shift_altgr_map)
{
    Syscall::SC_getkeymap_params params {
        map,
        shift_map,
        alt_map,
        altgr_map,
        shift_altgr_map,
        { name_buffer, name_buffer_size }
    };
    int rc = syscall(SC_getkeymap, &params);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param ptr 
 * @param count 
 * @return u16 
 */
u16 internet_checksum(void const* ptr, size_t count)
{
    u32 checksum = 0;
    auto* w = (u16 const*)ptr;
    while (count > 1) {
        checksum += ntohs(*w++);
        if (checksum & 0x80000000)
            checksum = (checksum & 0xffff) | (checksum >> 16);
        count -= 2;
    }
    while (checksum >> 16)
        checksum = (checksum & 0xffff) + (checksum >> 16);
    return htons(~checksum);
}

/**
 * @param command 
 * @param arg0 
 * @param arg1 
 * @return int 
 */
int emuctl(uintptr_t command, uintptr_t arg0, uintptr_t arg1)
{
    return syscall(SC_emuctl, command, arg0, arg1);
}

/**
 * @param path 
 * @param path_length 
 * @param options 
 * @param ... 
 * @return int 
 */
int pranaos_open(char const* path, size_t path_length, int options, ...)
{
    if (!path) {
        errno = EFAULT;
        return -1;
    }

    if (path_length > INT32_MAX) {
        errno = EINVAL;
        return -1;
    }

    va_list ap;
    va_start(ap, options);
    auto mode = (mode_t)va_arg(ap, unsigned);
    va_end(ap);

    Syscall::SC_open_params params { AT_FDCWD, { path, path_length }, options, mode };
    int rc = syscall(SC_open, &params);

    __RETURN_WITH_ERRNO(rc, rc, -1);
}
}
