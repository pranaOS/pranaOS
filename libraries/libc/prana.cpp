/**
 * @file prana.cpp
 * @author Krisna Pranav
 * @brief prana
 * @version 6.0
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav, 2000-2002 Opsycon AB (www.opsycon.se)
 * 
 */

#pragma once 

#include <errno.h>
#include <prana.h>
#include <kernel/api/syscall.h>

extern "C" 
{

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
     * @param path 
     * @param path_length 
     * @return int 
     */
    int module_load(const char* path, size_t path_length)
    {
        int rc = syscall(SC_module_load, path, path_length);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param name 
     * @param name_length 
     * @return int 
     */
    int module_unload(const char* name, size_t name_length)
    {
        int rc = syscall(SC_module_unload, name, name_length);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param pid 
     * @return int 
     */
    int profiling_enable(pid_t pid)
    {
        int rc = syscall(SC_profiling_enable, pid);
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
     * @param tid 
     * @param amount 
     * @return int 
     */
    int set_thread_boost(pid_t tid, int amount)
    {
        int rc = syscall(SC_set_thread_boost, tid, amount);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param tid 
     * @param amount 
     * @return int 
     */
    int set_process_boost(pid_t tid, int amount)
    {
        int rc = syscall(SC_set_process_boost, tid, amount);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param userspace_address 
     * @param futex_op 
     * @param value 
     * @param timeout 
     * @return int 
     */
    int futex(int32_t* userspace_address, int futex_op, int32_t value, const struct timespec* timeout)
    {
        Syscall::SC_futex_params params { userspace_address, futex_op, value, timeout };
        int rc = syscall(SC_futex, &params);
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
     * @param shbuf_id 
     * @param size 
     * @return void* 
     */
    void* shbuf_get(int shbuf_id, size_t* size)
    {
        ssize_t rc = syscall(SC_shbuf_get, shbuf_id, size);
        if (rc < 0 && -rc < EMAXERRNO) {
            errno = -rc;
            return (void*)-1;
        }
        return (void*)rc;
    }

    /**
     * @param shbuf_id 
     * @return int 
     */
    int shbuf_release(int shbuf_id)
    {
        int rc = syscall(SC_shbuf_release, shbuf_id);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param shbuf_id 
     * @return int 
     */
    int shbuf_seal(int shbuf_id)
    {
        int rc = syscall(SC_shbuf_seal, shbuf_id);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param size 
     * @param buffer 
     * @return int 
     */
    int shbuf_create(int size, void** buffer)
    {
        int rc = syscall(SC_shbuf_create, size, buffer);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param shbuf_id 
     * @param peer_pid 
     * @return int 
     */
    int shbuf_allow_pid(int shbuf_id, pid_t peer_pid)
    {
        int rc = syscall(SC_shbuf_allow_pid, shbuf_id, peer_pid);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param shbuf_id 
     * @return int 
     */
    int shbuf_allow_all(int shbuf_id)
    {
        int rc = syscall(SC_shbuf_allow_all, shbuf_id);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param user_stack_base 
     * @param user_stack_size 
     * @return int 
     */
    int get_stack_bounds(uintptr_t* user_stack_base, size_t* user_stack_size)
    {
        int rc = syscall(SC_get_stack_bounds, user_stack_base, user_stack_size);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

} // extern "C"
