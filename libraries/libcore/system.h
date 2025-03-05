/**
 * @file system.h
 * @author Krisna Pranav
 * @brief system
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/error.h>
#include <mods/stringview.h>
#include <dirent.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <signal.h>
#include <spawn.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <termios.h>
#include <time.h>
#include <utime.h>

#ifndef MODS_OS_BSD_GENERIC
#    include <shadow.h>
#endif

namespace Core::System
{

    #ifdef __pranaos__
    /**
     * @return ErrorOr<void> 
     */
    ErrorOr<void> beep();

    /**
     * @param promises 
     * @param execpromises 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> pledge(StringView promises, StringView execpromises = {});

    /**
     * @param path 
     * @param permissions 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> unveil(StringView path, StringView permissions);

    /**
     * @param sockfd 
     * @param fd 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> sendfd(int sockfd, int fd);

    /**
     * @param sockfd 
     * @param options 
     * @return ErrorOr<int> 
     */
    ErrorOr<int> recvfd(int sockfd, int options);

    /**
     * @param tid 
     * @param tracee_addr 
     * @param destination_buf 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> ptrace_peekbuf(pid_t tid, void const* tracee_addr, Bytes destination_buf);

    /**
     * @return ErrorOr<void> 
     */
    ErrorOr<void> setgroups(Span<gid_t const>);

    /**
     * @param source_fd 
     * @param target 
     * @param fs_type 
     * @param flags 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> mount(int source_fd, StringView target, StringView fs_type, int flags);

    /**
     * @param mount_point 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> umount(StringView mount_point);

    /**
     * @param request 
     * @param tid 
     * @param address 
     * @param data 
     * @return ErrorOr<long> 
     */
    ErrorOr<long> ptrace(int request, pid_t tid, void* address, void* data);

    /**
     * @param pid 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> disown(pid_t pid);

    /**
     * @param event_mask 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> profiling_enable(pid_t, u64 event_mask);

    /**
     * @return ErrorOr<void> 
     */
    ErrorOr<void> profiling_disable(pid_t);
    ErrorOr<void> profiling_free_buffer(pid_t);
    #endif

} // namespace Core::System
