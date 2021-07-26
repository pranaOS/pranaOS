/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <base/Types.h>
#include <base/Userspace.h>
#ifdef __pranaos__
#    include <libc/fd_set.h>
#endif

constexpr int syscall_vector = 0x82;
extern "C" {
struct pollfd;
struct timeval;
struct timespec;
struct sockaddr;
struct siginfo;
struct stat;
struct statvfs;
typedef u32 socklen_t;
}

namespace Kernel {
enum class NeedsBigProcessLock {
    Yes,
    No
};

#define ENUMERATE_SYSCALLS(S)                               \
S(yield, NeedsBigProcessLock::No)                       \
S(open, NeedsBigProcessLock::Yes)                       \
S(close, NeedsBigProcessLock::Yes)                      \
S(read, NeedsBigProcessLock::Yes)                       \
S(lseek, NeedsBigProcessLock::Yes)                      \
S(kill, NeedsBigProcessLock::Yes)                       \
S(getuid, NeedsBigProcessLock::Yes)                     \
S(exit, NeedsBigProcessLock::Yes)                       \
S(geteuid, NeedsBigProcessLock::Yes)                    \
S(getegid, NeedsBigProcessLock::Yes)                    \
S(getgid, NeedsBigProcessLock::Yes)                     \
S(getpid, NeedsBigProcessLock::No)                      \
S(getppid, NeedsBigProcessLock::Yes)                    \
S(getresuid, NeedsBigProcessLock::Yes)                  \
S(getresgid, NeedsBigProcessLock::Yes)                  \
S(waitid, NeedsBigProcessLock::Yes)                     \
S(mmap, NeedsBigProcessLock::Yes)                       \
S(munmap, NeedsBigProcessLock::Yes)                     \
S(get_dir_entries, NeedsBigProcessLock::Yes)            \
S(getcwd, NeedsBigProcessLock::Yes)                     \
S(gettimeofday, NeedsBigProcessLock::Yes)               \
S(gethostname, NeedsBigProcessLock::No)                 \
S(sethostname, NeedsBigProcessLock::No)                 \
S(chdir, NeedsBigProcessLock::Yes)                      \
S(uname, NeedsBigProcessLock::No)                       \
S(set_mmap_name, NeedsBigProcessLock::Yes)              \
S(readlink, NeedsBigProcessLock::Yes)                   \
S(write, NeedsBigProcessLock::Yes)                      \
S(ttyname, NeedsBigProcessLock::Yes)                    \
S(stat, NeedsBigProcessLock::Yes)                       \
S(getsid, NeedsBigProcessLock::Yes)                     \
S(setsid, NeedsBigProcessLock::Yes)                     \
S(getpgid, NeedsBigProcessLock::Yes)                    \
S(setpgid, NeedsBigProcessLock::Yes)                    \
S(getpgrp, NeedsBigProcessLock::Yes)                    \
S(fork, NeedsBigProcessLock::Yes)                       \
S(execve, NeedsBigProcessLock::Yes)                     \
S(dup2, NeedsBigProcessLock::Yes)                       \
S(sigaction, NeedsBigProcessLock::Yes)                  \
S(umask, NeedsBigProcessLock::Yes)                      \
S(getgroups, NeedsBigProcessLock::Yes)                  \
S(setgroups, NeedsBigProcessLock::Yes)                  \
S(sigreturn, NeedsBigProcessLock::Yes)                  \
S(sigprocmask, NeedsBigProcessLock::Yes)                \
S(sigpending, NeedsBigProcessLock::Yes)                 \
S(pipe, NeedsBigProcessLock::Yes)                       \
S(killpg, NeedsBigProcessLock::Yes)                     \
S(seteuid, NeedsBigProcessLock::Yes)                    \
S(setegid, NeedsBigProcessLock::Yes)                    \
S(setuid, NeedsBigProcessLock::Yes)                     \
S(setgid, NeedsBigProcessLock::Yes)                     \
S(setreuid, NeedsBigProcessLock::Yes)                   \
S(setresuid, NeedsBigProcessLock::Yes)                  \
S(setresgid, NeedsBigProcessLock::Yes)                  \
S(alarm, NeedsBigProcessLock::Yes)                      \
S(fstat, NeedsBigProcessLock::Yes)                      \
S(access, NeedsBigProcessLock::Yes)                     \
S(fcntl, NeedsBigProcessLock::Yes)                      \
S(ioctl, NeedsBigProcessLock::Yes)                      \
S(mkdir, NeedsBigProcessLock::Yes)                      \
S(times, NeedsBigProcessLock::Yes)                      \
S(utime, NeedsBigProcessLock::Yes)                      \
S(sync, NeedsBigProcessLock::Yes)                       \
S(ptsname, NeedsBigProcessLock::Yes)                    \
S(select, NeedsBigProcessLock::Yes)                     \
S(unlink, NeedsBigProcessLock::Yes)                     \
S(poll, NeedsBigProcessLock::Yes)                       \
S(rmdir, NeedsBigProcessLock::Yes)                      \
S(chmod, NeedsBigProcessLock::Yes)                      \
S(socket, NeedsBigProcessLock::Yes)                     \
S(bind, NeedsBigProcessLock::Yes)                       \
S(accept4, NeedsBigProcessLock::Yes)                    \
S(listen, NeedsBigProcessLock::Yes)                     \
S(connect, NeedsBigProcessLock::Yes)                    \
S(link, NeedsBigProcessLock::Yes)                       \
S(chown, NeedsBigProcessLock::Yes)                      \
S(fchmod, NeedsBigProcessLock::Yes)                     \
S(symlink, NeedsBigProcessLock::Yes)                    \
S(sendmsg, NeedsBigProcessLock::Yes)                    \
S(recvmsg, NeedsBigProcessLock::Yes)                    \
S(getsockopt, NeedsBigProcessLock::Yes)                 \
S(setsockopt, NeedsBigProcessLock::Yes)                 \
S(create_thread, NeedsBigProcessLock::Yes)              \
S(gettid, NeedsBigProcessLock::No)                      \
S(rename, NeedsBigProcessLock::Yes)                     \
S(ftruncate, NeedsBigProcessLock::Yes)                  \
S(exit_thread, NeedsBigProcessLock::Yes)                \
S(mknod, NeedsBigProcessLock::Yes)                      \
S(writev, NeedsBigProcessLock::Yes)                     \
S(beep, NeedsBigProcessLock::Yes)                       \
S(getsockname, NeedsBigProcessLock::Yes)                \
S(getpeername, NeedsBigProcessLock::Yes)                \
S(socketpair, NeedsBigProcessLock::Yes)                 \
S(sched_setparam, NeedsBigProcessLock::Yes)             \
S(sched_getparam, NeedsBigProcessLock::Yes)             \
S(fchown, NeedsBigProcessLock::Yes)                     \
S(halt, NeedsBigProcessLock::Yes)                       \
S(reboot, NeedsBigProcessLock::Yes)                     \
S(mount, NeedsBigProcessLock::Yes)                      \
S(umount, NeedsBigProcessLock::Yes)                     \
S(dump_backtrace, NeedsBigProcessLock::Yes)             \
S(dbgputch, NeedsBigProcessLock::Yes)                   \
S(dbgputstr, NeedsBigProcessLock::Yes)                  \
S(create_inode_watcher, NeedsBigProcessLock::Yes)       \
S(inode_watcher_add_watch, NeedsBigProcessLock::Yes)    \
S(inode_watcher_remove_watch, NeedsBigProcessLock::Yes) \
S(mprotect, NeedsBigProcessLock::Yes)                   \
S(realpath, NeedsBigProcessLock::Yes)                   \
S(get_process_name, NeedsBigProcessLock::Yes)           \
S(fchdir, NeedsBigProcessLock::Yes)                     \
S(getrandom, NeedsBigProcessLock::Yes)                  \
S(getkeymap, NeedsBigProcessLock::Yes)                  \
S(setkeymap, NeedsBigProcessLock::Yes)                  \
S(clock_gettime, NeedsBigProcessLock::Yes)              \
S(clock_settime, NeedsBigProcessLock::Yes)              \
S(clock_nanosleep, NeedsBigProcessLock::Yes)            \
S(join_thread, NeedsBigProcessLock::Yes)                \
S(module_load, NeedsBigProcessLock::Yes)                \
S(module_unload, NeedsBigProcessLock::Yes)              \
S(detach_thread, NeedsBigProcessLock::Yes)              \
S(set_thread_name, NeedsBigProcessLock::Yes)            \
S(get_thread_name, NeedsBigProcessLock::Yes)            \
S(madvise, NeedsBigProcessLock::Yes)                    \
S(purge, NeedsBigProcessLock::Yes)                      \
S(profiling_enable, NeedsBigProcessLock::Yes)           \
S(profiling_disable, NeedsBigProcessLock::Yes)          \
S(profiling_free_buffer, NeedsBigProcessLock::Yes)      \
S(futex, NeedsBigProcessLock::Yes)                      \
S(chroot, NeedsBigProcessLock::Yes)                     \
S(pledge, NeedsBigProcessLock::Yes)                     \
S(unveil, NeedsBigProcessLock::Yes)                     \
S(perf_event, NeedsBigProcessLock::Yes)                 \
S(shutdown, NeedsBigProcessLock::Yes)                   \
S(get_stack_bounds, NeedsBigProcessLock::Yes)           \
S(ptrace, NeedsBigProcessLock::Yes)                     \
S(sendfd, NeedsBigProcessLock::Yes)                     \
S(recvfd, NeedsBigProcessLock::Yes)                     \
S(sysconf, NeedsBigProcessLock::Yes)                    \
S(set_process_name, NeedsBigProcessLock::Yes)           \
S(disown, NeedsBigProcessLock::Yes)                     \
S(adjtime, NeedsBigProcessLock::Yes)                    \
S(allocate_tls, NeedsBigProcessLock::Yes)               \
S(prctl, NeedsBigProcessLock::Yes)                      \
S(mremap, NeedsBigProcessLock::Yes)                     \
S(set_coredump_metadata, NeedsBigProcessLock::Yes)      \
S(anon_create, NeedsBigProcessLock::Yes)                \
S(msyscall, NeedsBigProcessLock::Yes)                   \
S(readv, NeedsBigProcessLock::Yes)                      \
S(emuctl, NeedsBigProcessLock::Yes)                     \
S(statvfs, NeedsBigProcessLock::Yes)                    \
S(fstatvfs, NeedsBigProcessLock::Yes)                   \
S(kill_thread, NeedsBigProcessLock::Yes)
namespace Syscall {
enum Function {
#undef __ENUMERATE_SYSCALL
#define __ENUMERATE_SYSCALL(sys_call, needs_lock) SC_##sys_call,
ENUMERATE_SYSCALLS(__ENUMERATE_SYSCALL)
#undef __ENUMERATE_SYSCALL
        __Count
};
constexpr const char* to_string(Function function)
{
switch (function) {
#undef __ENUMERATE_SYSCALL
#define __ENUMERATE_SYSCALL(sys_call, needs_lock) \
case SC_##sys_call:                           \
return #sys_call;
ENUMERATE_SYSCALLS(__ENUMERATE_SYSCALL)
#undef __ENUMERATE_SYSCALL
default:
break;
    }
return "Unknown";
}
#ifdef __pranaos__
struct StringArgument {
const char* characters;
size_t length { 0 };
};


}