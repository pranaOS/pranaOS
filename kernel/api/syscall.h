/**
 * @file syscall.h
 * @author Krisna Pranav
 * @brief syscall
 * @version 6.0
 * @date 2023-07-09
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>
#include <mods/usrspace.h>

#ifdef __prana__
#    include <libraries/libc/fd_set.h>
#endif

constexpr int syscall_vector = 0x82;

extern "C" {
    struct pollfd;
    struct timeval;
    struct timespec;
    struct sockaddr;
    struct siginfo;
    struct stat;
    typedef u32 socklen_t;
}

namespace Kernel {

    #define ENUMERATE_SYSCALLS(S) \
        S(yield)                  \
        S(open)                   \
        S(close)                  \
        S(read)                   \
        S(lseek)                  \
        S(kill)                   \
        S(getuid)                 \
        S(exit)                   \
        S(geteuid)                \
        S(getegid)                \
        S(getgid)                 \
        S(getpid)                 \
        S(getppid)                \
        S(getresuid)              \
        S(getresgid)              \
        S(waitid)                 \
        S(mmap)                   \
        S(munmap)                 \
        S(get_dir_entries)        \
        S(getcwd)                 \
        S(gettimeofday)           \
        S(gethostname)            \
        S(sethostname)            \
        S(chdir)                  \
        S(uname)                  \
        S(set_mmap_name)          \
        S(readlink)               \
        S(write)                  \
        S(ttyname)                \
        S(stat)                   \
        S(getsid)                 \
        S(setsid)                 \
        S(getpgid)                \
        S(setpgid)                \
        S(getpgrp)                \
        S(fork)                   \
        S(execve)                 \
        S(dup2)                   \
        S(sigaction)              \
        S(umask)                  \
        S(getgroups)              \
        S(setgroups)              \
        S(sigreturn)              \
        S(sigprocmask)            \
        S(sigpending)             \
        S(pipe)                   \
        S(killpg)                 \
        S(seteuid)                \
        S(setegid)                \
        S(setuid)                 \
        S(setgid)                 \
        S(setresuid)              \
        S(setresgid)              \
        S(alarm)                  \
        S(fstat)                  \
        S(access)                 \
        S(fcntl)                  \
        S(ioctl)                  \
        S(mkdir)                  \
        S(times)                  \
        S(utime)                  \
        S(sync)                   \
        S(ptsname)                \
        S(select)                 \
        S(unlink)                 \
        S(poll)                   \
        S(rmdir)                  \
        S(chmod)                  \
        S(socket)                 \
        S(bind)                   \
        S(accept)                 \
        S(listen)                 \
        S(connect)                \
        S(shbuf_create)           \
        S(shbuf_allow_pid)        \
        S(shbuf_get)              \
        S(shbuf_release)          \
        S(link)                   \
        S(chown)                  \
        S(fchmod)                 \
        S(symlink)                \
        S(shbuf_seal)             \
        S(sendmsg)                \
        S(recvmsg)                \
        S(getsockopt)             \
        S(setsockopt)             \
        S(create_thread)          \
        S(gettid)                 \
        S(donate)                 \
        S(rename)                 \
        S(ftruncate)              \
        S(exit_thread)            \
        S(mknod)                  \
        S(writev)                 \
        S(beep)                   \
        S(getsockname)            \
        S(getpeername)            \
        S(sched_setparam)         \
        S(sched_getparam)         \
        S(fchown)                 \
        S(halt)                   \
        S(reboot)                 \
        S(mount)                  \
        S(umount)                 \
        S(dump_backtrace)         \
        S(dbgputch)               \
        S(dbgputstr)              \
        S(watch_file)             \
        S(shbuf_allow_all)        \
        S(set_process_icon)       \
        S(mprotect)               \
        S(realpath)               \
        S(get_process_name)       \
        S(fchdir)                 \
        S(getrandom)              \
        S(setkeymap)              \
        S(clock_gettime)          \
        S(clock_settime)          \
        S(clock_nanosleep)        \
        S(join_thread)            \
        S(module_load)            \
        S(module_unload)          \
        S(detach_thread)          \
        S(set_thread_name)        \
        S(get_thread_name)        \
        S(madvise)                \
        S(purge)                  \
        S(shbuf_set_volatile)     \
        S(profiling_enable)       \
        S(profiling_disable)      \
        S(futex)                  \
        S(set_thread_boost)       \
        S(set_process_boost)      \
        S(chroot)                 \
        S(pledge)                 \
        S(unveil)                 \
        S(perf_event)             \
        S(shutdown)               \
        S(get_stack_bounds)       \
        S(ptrace)                 \
        S(minherit)               \
        S(sendfd)                 \
        S(recvfd)                 \
        S(sysconf)                \
        S(set_process_name)       \
        S(disown)                 \
        S(adjtime)                \
        S(allocate_tls)

    namespace Syscall {

        enum Function {
        #undef __ENUMERATE_SYSCALL
        #define __ENUMERATE_SYSCALL(x) SC_##x,
            ENUMERATE_SYSCALLS(__ENUMERATE_SYSCALL)
        #undef __ENUMERATE_SYSCALL
                __Count
        };

        /**
         * @param function 
         * @return constexpr const char* 
         */
        constexpr const char* to_string(Function function) {
            switch (function) {
        #undef __ENUMERATE_SYSCALL
        #define __ENUMERATE_SYSCALL(x) \
            case SC_##x:               \
                return #x;
                ENUMERATE_SYSCALLS(__ENUMERATE_SYSCALL)
        #undef __ENUMERATE_SYSCALL
            default:
                break;
            }
            return "Unknown";
        }

        #ifdef __prana__
        struct StringArgument {
            const char* characters;
            size_t length { 0 };
        };

        template<typename DataType, typename SizeType>
        struct MutableBufferArgument {
            DataType* data { nullptr };
            SizeType size { 0 };
        };

        template<typename DataType, typename SizeType>
        struct ImmutableBufferArgument {
            const DataType* data { nullptr };
            SizeType size { 0 };
        };

        struct StringListArgument {
            StringArgument* strings {};
            size_t length { 0 };
        };

        struct SC_mmap_params {
            uintptr_t addr;
            size_t size;
            size_t alignment;
            int32_t prot;
            int32_t flags;
            int32_t fd;
            ssize_t offset;
            StringArgument name;
        };

        struct SC_open_params {
            int dirfd;
            StringArgument path;
            int options;
            u16 mode;
        };

        struct SC_select_params {
            int nfds;
            fd_set* readfds;
            fd_set* writefds;
            fd_set* exceptfds;
            const struct timespec* timeout;
            const u32* sigmask;
        };

        struct SC_poll_params {
            struct pollfd* fds;
            unsigned nfds;
            const struct timespec* timeout;
            const u32* sigmask;
        };

        struct SC_clock_nanosleep_params {
            int clock_id;
            int flags;
            const struct timespec* requested_sleep;
            struct timespec* remaining_sleep;
        };

        struct SC_getsockopt_params {
            int sockfd;
            int level;
            int option;
            void* value;
            socklen_t* value_size;
        };

        struct SC_setsockopt_params {
            int sockfd;
            int level;
            int option;
            const void* value;
            socklen_t value_size;
        };

        struct SC_getsockname_params {
            int sockfd;
            sockaddr* addr;
            socklen_t* addrlen;
        };

        struct SC_getpeername_params {
            int sockfd;
            sockaddr* addr;
            socklen_t* addrlen;
        };

        struct SC_futex_params {
            const i32* userspace_address;
            int futex_op;
            i32 val;
            const timespec* timeout;
        };

        struct SC_setkeymap_params {
            const u32* map;
            const u32* shift_map;
            const u32* alt_map;
            const u32* altgr_map;
            StringArgument map_name;
        };

        struct SC_create_thread_params {
            unsigned int m_detach_state = 0;
            int m_schedule_priority = 30;   
            unsigned int m_guard_page_size = 0;          
            unsigned int m_reported_guard_page_size = 0; 
            unsigned int m_stack_size = 4 * MiB;         
            void* m_stack_location;                      
        };

        struct SC_realpath_params {
            StringArgument path;
            MutableBufferArgument<char, size_t> buffer;
        };

        struct SC_set_mmap_name_params {
            void* addr;
            size_t size;
            StringArgument name;
        };

        struct SC_execve_params {
            StringArgument path;
            StringListArgument arguments;
            StringListArgument environment;
        };

        struct SC_readlink_params {
            StringArgument path;
            MutableBufferArgument<char, size_t> buffer;
        };

        struct SC_link_params {
            StringArgument old_path;
            StringArgument new_path;
        };

        struct SC_chown_params {
            StringArgument path;
            u32 uid;
            u32 gid;
        };

        struct SC_mknod_params {
            StringArgument path;
            u16 mode;
            u32 dev;
        };

        struct SC_symlink_params {
            StringArgument target;
            StringArgument linkpath;
        };

        struct SC_rename_params {
            StringArgument old_path;
            StringArgument new_path;
        };

        struct SC_mount_params {
            int source_fd;
            StringArgument target;
            StringArgument fs_type;
            int flags;
        };

        struct SC_pledge_params {
            StringArgument promises;
            StringArgument execpromises;
        };

        struct SC_unveil_params {
            StringArgument path;
            StringArgument permissions;
        };

        struct SC_waitid_params {
            int idtype;
            int id;
            struct siginfo* infop;
            int options;
        };

        struct SC_stat_params {
            StringArgument path;
            struct stat* statbuf;
            bool follow_symlinks;
        };

        struct SC_ptrace_params {
            int request;
            pid_t tid;
            u8* addr;
            int data;
        };

        struct SC_ptrace_peek_params {
            const u32* address;
            u32* out_data;
        };

        void initialize();
        int sync();

        /**
         * @param function 
         * @return uintptr_t 
         */
        inline uintptr_t invoke(Function function){
            uintptr_t result;
            asm volatile("int $0x82"
                        : "=a"(result)
                        : "a"(function)
                        : "memory");
            return result;
        }

        /**
         * @tparam T1 
         * @param function 
         * @param arg1 
         * @return uintptr_t 
         */
        template<typename T1>
        inline uintptr_t invoke(Function function, T1 arg1) {
            uintptr_t result;
            asm volatile("int $0x82"
                        : "=a"(result)
                        : "a"(function), "d"((uintptr_t)arg1)
                        : "memory");
            return result;
        }

        /**
         * @tparam T1 
         * @tparam T2 
         * @param function 
         * @param arg1 
         * @param arg2 
         * @return uintptr_t 
         */
        template<typename T1, typename T2>
        inline uintptr_t invoke(Function function, T1 arg1, T2 arg2) {
            uintptr_t result;
            asm volatile("int $0x82"
                        : "=a"(result)
                        : "a"(function), "d"((uintptr_t)arg1), "c"((uintptr_t)arg2)
                        : "memory");
            return result;
        }

        /**
         * @tparam T1 
         * @tparam T2 
         * @tparam T3 
         * @param function 
         * @param arg1 
         * @param arg2 
         * @param arg3 
         * @return uintptr_t 
         */
        template<typename T1, typename T2, typename T3>
        inline uintptr_t invoke(Function function, T1 arg1, T2 arg2, T3 arg3) {
            uintptr_t result;
            asm volatile("int $0x82"
                        : "=a"(result)
                        : "a"(function), "d"((uintptr_t)arg1), "c"((uintptr_t)arg2), "b"((uintptr_t)arg3)
                        : "memory");
            return result;
        }
        #endif

    }

    #undef __ENUMERATE_SYSCALL
    #define __ENUMERATE_SYSCALL(x) using Syscall::SC_##x;
    ENUMERATE_SYSCALLS(__ENUMERATE_SYSCALL)
    #undef __ENUMERATE_SYSCALL
    #define syscall Syscall::invoke

}

using namespace Kernel;
