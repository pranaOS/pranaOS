/**
 * @file process.h
 * @author Krisna Pranav
 * @brief process
 * @version 6.0
 * @date 2023-08-12
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <libc/signal_codes.h>
#include <libelf/auxvector.h>
#include <mods/hashmap.h>
#include <mods/check.h>
#include <mods/inlinelinkedlist.h>
#include <mods/nonnullownptrvector.h>
#include <mods/nonnullrefptrvector.h>
#include <mods/string.h>
#include <mods/usrspace.h>
#include <mods/weakable.h>
#include <mods/weakptr.h>
#include <kernel/forward.h>
#include <kernel/lock.h>
#include <kernel/processgroup.h>
#include <kernel/stdlib.h>
#include <kernel/thread.h>
#include <kernel/threadtracer.h>
#include <kernel/unixtypes.h>
#include <kernel/api/syscall.h>
#include <kernel/filesystem/inodemetadata.h>
#include <kernel/vm/rangeallocator.h>

namespace ELF 
{
    class Loader;
} // namespace ELF

namespace Kernel 
{

    /**
     * @return timeval 
     */
    timeval kgettimeofday();

    /// kgettimeofday
    void kgettimeofday(timeval&);

    extern VirtualAddress g_return_to_ring3_from_signal_trampoline;

    #define ENUMERATE_PLEDGE_PROMISES         \
        __ENUMERATE_PLEDGE_PROMISE(stdio)     \
        __ENUMERATE_PLEDGE_PROMISE(rpath)     \
        __ENUMERATE_PLEDGE_PROMISE(wpath)     \
        __ENUMERATE_PLEDGE_PROMISE(cpath)     \
        __ENUMERATE_PLEDGE_PROMISE(dpath)     \
        __ENUMERATE_PLEDGE_PROMISE(inet)      \
        __ENUMERATE_PLEDGE_PROMISE(id)        \
        __ENUMERATE_PLEDGE_PROMISE(proc)      \
        __ENUMERATE_PLEDGE_PROMISE(exec)      \
        __ENUMERATE_PLEDGE_PROMISE(unix)      \
        __ENUMERATE_PLEDGE_PROMISE(recvfd)    \
        __ENUMERATE_PLEDGE_PROMISE(sendfd)    \
        __ENUMERATE_PLEDGE_PROMISE(fattr)     \
        __ENUMERATE_PLEDGE_PROMISE(tty)       \
        __ENUMERATE_PLEDGE_PROMISE(chown)     \
        __ENUMERATE_PLEDGE_PROMISE(chroot)    \
        __ENUMERATE_PLEDGE_PROMISE(thread)    \
        __ENUMERATE_PLEDGE_PROMISE(video)     \
        __ENUMERATE_PLEDGE_PROMISE(accept)    \
        __ENUMERATE_PLEDGE_PROMISE(settime)   \
        __ENUMERATE_PLEDGE_PROMISE(sigaction) \
        __ENUMERATE_PLEDGE_PROMISE(setkeymap) \
        __ENUMERATE_PLEDGE_PROMISE(shared_buffer)

    enum class Pledge : u32 
    {
    #define __ENUMERATE_PLEDGE_PROMISE(x) x,
        ENUMERATE_PLEDGE_PROMISES
    #undef __ENUMERATE_PLEDGE_PROMISE
    }; // enum Pledge

    enum class VeilState 
    {
        None,
        Dropped,
        Locked,
    }; // enum VeilState

    struct UnveiledPath 
    {
        enum Access 
        {
            Read = 1,
            Write = 2,
            Execute = 4,
            CreateOrRemove = 8,
            Browse = 16,
        };

        String path;

        unsigned permissions { 0 };
    };

    class Process
        : public RefCounted<Process>
        , public InlineLinkedListNode<Process>
        , public Weakable<Process> {

        MOD_MAKE_NONCOPYABLE(Process);
        MOD_MAKE_NONMOVABLE(Process);

        friend class InlineLinkedListNode<Process>;
        friend class Thread;
        friend class CoreDump;

    public:
        /**
         * @return Process* 
         */
        inline static Process* current()
        {
            auto current_thread = Processor::current().current_thread();

            return current_thread ? &current_thread->process() : nullptr;
        }

        /**
         * @tparam EntryFunction 
         * @param first_thread 
         * @param name 
         * @param entry 
         * @param affinity 
         * @return NonnullRefPtr<Process> 
         */
        template<typename EntryFunction>
        static NonnullRefPtr<Process> create_kernel_process(RefPtr<Thread>& first_thread, String&& name, EntryFunction entry, u32 affinity = THREAD_AFFINITY_DEFAULT)
        {
            auto* entry_func = new EntryFunction(move(entry));

            return create_kernel_process(
                first_thread, move(name), [](void* data) {
                    EntryFunction* func = reinterpret_cast<EntryFunction*>(data);
                    (*func)();
                    delete func;
                },
                entry_func, affinity);
        }

        /**
         * @param first_thread 
         * @param name 
         * @param entry 
         * @param entry_data 
         * @param affinity 
         * @return NonnullRefPtr<Process> 
         */
        static NonnullRefPtr<Process> create_kernel_process(RefPtr<Thread>& first_thread, String&& name, void (*entry)(void*), void* entry_data = nullptr, u32 affinity = THREAD_AFFINITY_DEFAULT);

        /**
         * @param first_thread 
         * @param path 
         * @param ppid 
         * @param error 
         * @param arguments 
         * @param environment 
         * @return RefPtr<Process> 
         */
        static RefPtr<Process> create_user_process(RefPtr<Thread>& first_thread, const String& path, uid_t, gid_t, ProcessID ppid, int& error, Vector<String>&& arguments = Vector<String>(), Vector<String>&& environment = Vector<String>(), TTY* = nullptr);

        /// @brief Destroy the Process object
        ~Process();

        /**
         * @return Vector<ProcessID> 
         */
        static Vector<ProcessID> all_pids();

        /**
         * @return Mods::NonnullRefPtrVector<Process> 
         */
        static Mods::NonnullRefPtrVector<Process> all_processes();

        /**
         * @tparam EntryFunction 
         * @param entry 
         * @param priority 
         * @param name 
         * @param affinity 
         * @param joinable 
         * @return RefPtr<Thread> 
         */
        template<typename EntryFunction>
        RefPtr<Thread> create_kernel_thread(EntryFunction entry, u32 priority, const String& name, u32 affinity = THREAD_AFFINITY_DEFAULT, bool joinable = true)
        {
            auto* entry_func = new EntryFunction(move(entry));
            
            return create_kernel_thread([](void* data) {
                EntryFunction* func = reinterpret_cast<EntryFunction*>(data);
                (*func)();
                delete func;
            },
                priority, name, affinity, joinable);
        }

        /**
         * @param entry 
         * @param entry_data 
         * @param priority 
         * @param name 
         * @param affinity 
         * @param joinable 
         * @return RefPtr<Thread> 
         */
        RefPtr<Thread> create_kernel_thread(void (*entry)(void*), void* entry_data, u32 priority, const String& name, u32 affinity = THREAD_AFFINITY_DEFAULT, bool joinable = true);

        /**
         * @return true 
         * @return false 
         */
        bool is_profiling() const 
        { 
            return m_profiling; 
        }

        /**
         * @param profiling 
         */
        void set_profiling(bool profiling) 
        { 
            m_profiling = profiling; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool should_core_dump() const 
        { 
            return m_should_dump_core; 
        }

        /**
         * @param dump_core 
         */
        void set_dump_core(bool dump_core) 
        { 
            m_should_dump_core = dump_core; 
        }

        /**
         * @return KBuffer 
         */
        KBuffer backtrace() const;

        /**
         * @return true 
         * @return false 
         */
        bool is_dead() const 
        { 
            return m_dead; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_stopped() const 
        { 
            return m_is_stopped.load(Mods::MemoryOrder::memory_order_relaxed); 
        }

        /**
         * @param stopped 
         * @return true 
         * @return false 
         */
        bool set_stopped(bool stopped) 
        { 
            return m_is_stopped.exchange(stopped, Mods::MemoryOrder::memory_order_relaxed); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_kernel_process() const 
        { 
            return m_is_kernel_process; 
        }

        bool is_user_process() const 
        { 
            return !m_is_kernel_process; 
        }

        /**
         * @return PageDirectory& 
         */
        PageDirectory& page_directory() 
        { 
            return *m_page_directory; 
        }

        /**
         * @return const PageDirectory& 
         */
        const PageDirectory& page_directory() const 
        { 
            return *m_page_directory; 
        }

        /**
         * @return RefPtr<Process> 
         */
        static RefPtr<Process> from_pid(ProcessID);

        /**
         * @param pgid 
         * @return SessionID 
         */
        static SessionID get_sid_from_pgid(ProcessGroupID pgid);

        /**
         * @return const String& 
         */
        const String& name() const 
        { 
            return m_name; 
        }

        /**
         * @return ProcessID 
         */
        ProcessID pid() const 
        { 
            return m_pid; 
        }

        /**
         * @return SessionID 
         */
        SessionID sid() const 
        { 
            return m_sid; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_session_leader() const 
        { 
            return m_sid.value() == m_pid.value(); 
        }

        /**
         * @return ProcessGroupID 
         */
        ProcessGroupID pgid() const 
        { 
            return m_pg ? m_pg->pgid() : 0; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_group_leader() const 
        { 
            return pgid().value() == m_pid.value(); 
        }

        /**
         * @return Span<const gid_t> 
         */
        Span<const gid_t> extra_gids() const 
        { 
            return m_extra_gids; 
        }

        uid_t euid() const 
        { 
            return m_euid; 
        }

        gid_t egid() const 
        { 
            return m_egid; 
        }

        uid_t uid() const 
        { 
            return m_uid; 
        }

        gid_t gid() const 
        { 
            return m_gid; 
        }

        uid_t suid() const 
        { 
            return m_suid; 
        }

        gid_t sgid() const 
        { 
            return m_sgid; 
        }

        /**
         * @return ProcessID 
         */
        ProcessID ppid() const 
        { 
            return m_ppid; 
        }

        /**
         * @return ThreadID 
         */
        ThreadID exec_tid() const 
        { 
            return m_exec_tid; 
        }

        mode_t umask() const 
        { 
            return m_umask; 
        }

        bool in_group(gid_t) const;

        /**
         * @param fd 
         * @return RefPtr<FileDescription> 
         */
        RefPtr<FileDescription> file_description(int fd) const;

        /**
         * @param fd 
         * @return int 
         */
        int fd_flags(int fd) const;

        template<typename Callback>
        static void for_each(Callback);

        template<typename Callback>
        static void for_each_in_pgrp(ProcessGroupID, Callback);

        template<typename Callback>
        void for_each_child(Callback);

        template<typename Callback>
        void for_each_thread(Callback) const;

        void die();
        void finalize();

        /**
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE SpinLock<u32>& get_lock() const 
        { 
            return m_lock; 
        }

        /**
         * @return ThreadTracer* 
         */
        ThreadTracer* tracer() 
        { 
            return m_tracer.ptr(); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_traced() const 
        { 
            return !!m_tracer; 
        }

        /**
         * @param tracer 
         */
        void start_tracing_from(ProcessID tracer);

        void stop_tracing();

        /// @brief: tracer_trap
        void tracer_trap(Thread&, const RegisterState&);

        int sys$yield();
        int sys$sync();
        int sys$beep();

        /// @brief: get process name
        int sys$get_process_name(Userspace<char*> buffer, size_t buffer_size);

        /// @brief: set process name
        int sys$set_process_name(Userspace<const char*> user_name, size_t user_name_length);

        /// @brief: watch file
        int sys$watch_file(Userspace<const char*> path, size_t path_length);

        /// @brief dbgputch
        int sys$dbgputch(u8);
        
        /// @brief dbgputstr
        int sys$dbgputstr(Userspace<const u8*>, int length);

        /// @brief dump_backtrace
        int sys$dump_backtrace();

        /// @brief gettid
        pid_t sys$gettid();

        /// @brief donate
        int sys$donate(pid_t tid);

        /// @brief ftruncate
        int sys$ftruncate(int fd, off_t);

        /// @brief setsid
        pid_t sys$setsid();

        /// @brief getsid
        pid_t sys$getsid(pid_t);

        /// @brief setpgid
        int sys$setpgid(pid_t pid, pid_t pgid);

        /// @brief getpgrp
        pid_t sys$getpgrp();

        /// @brief getpgid
        pid_t sys$getpgid(pid_t);

        /// @brief getuid
        uid_t sys$getuid();

        /// @brief getgid
        gid_t sys$getgid();

        /// @brief geteuid
        uid_t sys$geteuid();

        /// @brief getegid
        gid_t sys$getegid();

        /// @brief getpid
        pid_t sys$getpid();

        /// @brief getppid
        pid_t sys$getppid();

        /// @brief getresuid
        int sys$getresuid(Userspace<uid_t*>, Userspace<uid_t*>, Userspace<uid_t*>);

        /// @brief getresgid
        int sys$getresgid(Userspace<gid_t*>, Userspace<gid_t*>, Userspace<gid_t*>);

        /// @brief umask
        mode_t sys$umask(mode_t);

        /// @brief open
        int sys$open(Userspace<const Syscall::SC_open_params*>);

        /// @brief close
        int sys$close(int fd);

        /// @brief read
        ssize_t sys$read(int fd, Userspace<u8*>, ssize_t);

        /// @brief write
        ssize_t sys$write(int fd, const u8*, ssize_t);

        /// @brief writev
        ssize_t sys$writev(int fd, Userspace<const struct iovec*> iov, int iov_count);

        /// @brief fstat
        int sys$fstat(int fd, Userspace<stat*>);

        /// @brief stat
        int sys$stat(Userspace<const Syscall::SC_stat_params*>);

        /// @brief lseek
        int sys$lseek(int fd, off_t, int whence);

        /// @brief kill
        int sys$kill(pid_t pid_or_pgid, int sig);

        /// @brief exit
        [[noreturn]] void sys$exit(int status);

        /// @brief sigreturn
        int sys$sigreturn(RegisterState& registers);

        /// @brief waitid
        pid_t sys$waitid(Userspace<const Syscall::SC_waitid_params*>);

        /// @brief mmap
        void* sys$mmap(Userspace<const Syscall::SC_mmap_params*>);

        /// @brief munmap
        int sys$munmap(void*, size_t size);

        /// @brief set mmap name
        int sys$set_mmap_name(Userspace<const Syscall::SC_set_mmap_name_params*>);

        /// @brief mprotect
        int sys$mprotect(void*, size_t, int prot);

        /// @brief madvise
        int sys$madvise(void*, size_t, int advice);

        /// @brief minherit
        int sys$minherit(void*, size_t, int inherit);

        /// @brief purge
        int sys$purge(int mode);

        /// @brief select
        int sys$select(const Syscall::SC_select_params*);

        /// @brief poll
        int sys$poll(Userspace<const Syscall::SC_poll_params*>);

        /// @brief get_dir_entries
        ssize_t sys$get_dir_entries(int fd, void*, ssize_t);

        /// @brief get cwd
        int sys$getcwd(Userspace<char*>, ssize_t);

        /// @brief chdir
        int sys$chdir(Userspace<const char*>, size_t);

        /// @brief fchdir
        int sys$fchdir(int fd);

        /// @brief sleep
        int sys$sleep(unsigned seconds);

        /// @brief usleep
        int sys$usleep(useconds_t usec);

        /// @brief adjtime
        int sys$adjtime(Userspace<const timeval*>, Userspace<timeval*>);

        /// @brief gettimeofday
        int sys$gettimeofday(Userspace<timeval*>);

        /// @brief clock gettime
        int sys$clock_gettime(clockid_t, Userspace<timespec*>);

        /// @brief clock settime 
        int sys$clock_settime(clockid_t, Userspace<const timespec*>);

        /// @brief clock nanosleep
        int sys$clock_nanosleep(Userspace<const Syscall::SC_clock_nanosleep_params*>);

        /// @brief get hostname
        int sys$gethostname(Userspace<char*>, ssize_t);

        /// @brief sethostname
        int sys$sethostname(Userspace<const char*>, ssize_t);

        /// @brief uname
        int sys$uname(Userspace<utsname*>);

        /// @brief readlink
        int sys$readlink(Userspace<const Syscall::SC_readlink_params*>);

        /// @brief ttyname
        int sys$ttyname(int fd, Userspace<char*>, size_t);

        /// @brief ptsname
        int sys$ptsname(int fd, Userspace<char*>, size_t);

        /// @brief fork
        pid_t sys$fork(RegisterState&);

        /// @brief execve
        int sys$execve(Userspace<const Syscall::SC_execve_params*>);

        /// @brief dup2
        int sys$dup2(int old_fd, int new_fd);

        /// @brief sigaction
        int sys$sigaction(int signum, const sigaction* act, sigaction* old_act);

        /// @brief sigprocmask
        int sys$sigprocmask(int how, Userspace<const sigset_t*> set, Userspace<sigset_t*> old_set);

        /// @brief sigpending
        int sys$sigpending(Userspace<sigset_t*>);

        /// @brief getgroups
        int sys$getgroups(ssize_t, Userspace<gid_t*>);

        /// @brief setgroups
        int sys$setgroups(ssize_t, Userspace<const gid_t*>);

        /// @brief pipe
        int sys$pipe(int pipefd[2], int flags);

        /// @brief killpg
        int sys$killpg(pid_t pgrp, int sig);

        /// @brief seteuid
        int sys$seteuid(uid_t);
        
        /// @brief setegid
        int sys$setegid(gid_t);

        /// @brief setuid
        int sys$setuid(uid_t);

        /// @brief setgid
        int sys$setgid(gid_t);

        /// @brief setresuid
        int sys$setresuid(uid_t, uid_t, uid_t);

        /// @brief setresgid
        int sys$setresgid(gid_t, gid_t, gid_t);

        /// @brief alaram
        unsigned sys$alarm(unsigned seconds);

        /// @brief access
        int sys$access(Userspace<const char*> pathname, size_t path_length, int mode);

        /// @brief fcntl
        int sys$fcntl(int fd, int cmd, u32 extra_arg);

        /// @brief ioctl
        int sys$ioctl(int fd, unsigned request, FlatPtr arg);

        /// @brief mkdir
        int sys$mkdir(Userspace<const char*> pathname, size_t path_length, mode_t mode);

        /// @brief times
        clock_t sys$times(Userspace<tms*>);

        /// @brief utime
        int sys$utime(Userspace<const char*> pathname, size_t path_length, Userspace<const struct utimbuf*>);

        /// @brief link
        int sys$link(Userspace<const Syscall::SC_link_params*>);

        /// @brief unlink
        int sys$unlink(Userspace<const char*> pathname, size_t path_length);

        /// @brief symlink
        int sys$symlink(Userspace<const Syscall::SC_symlink_params*>);
        
        /// @brief rmdir
        int sys$rmdir(Userspace<const char*> pathname, size_t path_length);

        /// @brief mount
        int sys$mount(Userspace<const Syscall::SC_mount_params*>);

        /// @brief unmount
        int sys$umount(Userspace<const char*> mountpoint, size_t mountpoint_length);

        /// @brief chmod
        int sys$chmod(Userspace<const char*> pathname, size_t path_length, mode_t);

        /// @brief fchmod
        int sys$fchmod(int fd, mode_t);

        /// @brief chown
        int sys$chown(Userspace<const Syscall::SC_chown_params*>);

        /// @brief fchown
        int sys$fchown(int fd, uid_t, gid_t);

        /// @brief socket
        int sys$socket(int domain, int type, int protocol);

        /// @brief bind
        int sys$bind(int sockfd, Userspace<const sockaddr*> addr, socklen_t);

        /// @brief listen
        int sys$listen(int sockfd, int backlog);

        /// @brief accept
        int sys$accept(int sockfd, Userspace<sockaddr*>, Userspace<socklen_t*>);

        /// @brief connect
        int sys$connect(int sockfd, Userspace<const sockaddr*>, socklen_t);

        /// @brief shutdown
        int sys$shutdown(int sockfd, int how);

        /// @brief sendmsg
        ssize_t sys$sendmsg(int sockfd, Userspace<const struct msghdr*>, int flags);

        /// @brief recvmsg
        ssize_t sys$recvmsg(int sockfd, Userspace<struct msghdr*>, int flags);

        /// @brief getsockopt
        int sys$getsockopt(Userspace<const Syscall::SC_getsockopt_params*>);

        /// @brief setsockopt
        int sys$setsockopt(Userspace<const Syscall::SC_setsockopt_params*>);

        /// @brief getsockname
        int sys$getsockname(Userspace<const Syscall::SC_getsockname_params*>);

        /// @brief getpeername
        int sys$getpeername(Userspace<const Syscall::SC_getpeername_params*>);

        /// @brief sched_setparam
        int sys$sched_setparam(pid_t pid, Userspace<const struct sched_param*>);
        
        /// @brief sched getparam
        int sys$sched_getparam(pid_t pid, Userspace<struct sched_param*>);

        /// @brief create thread
        int sys$create_thread(void* (*)(void*), Userspace<const Syscall::SC_create_thread_params*>);

        /// @brief exit thread
        void sys$exit_thread(Userspace<void*>);

        /// @brief join thread
        int sys$join_thread(pid_t tid, Userspace<void**> exit_value);

        /// @brief detach thread
        int sys$detach_thread(pid_t tid);

        /// @brief set thread name
        int sys$set_thread_name(pid_t tid, Userspace<const char*> buffer, size_t buffer_size);

        /// @brief get thread name
        int sys$get_thread_name(pid_t tid, Userspace<char*> buffer, size_t buffer_size);

        /// @brief rename
        int sys$rename(Userspace<const Syscall::SC_rename_params*>);

        /// @brief mknod
        int sys$mknod(Userspace<const Syscall::SC_mknod_params*>);

        /// @brief shbuf create
        int sys$shbuf_create(int, void** buffer);

        /// @brief shbuf allow pid
        int sys$shbuf_allow_pid(int, pid_t peer_pid);

        /// @brief shbuf allow all
        int sys$shbuf_allow_all(int);

        /// @brief shbuf get 
        void* sys$shbuf_get(int shbuf_id, Userspace<size_t*> size);

        /// @brief shbuf release
        int sys$shbuf_release(int shbuf_id);

        /// @brief shbuf seal
        int sys$shbuf_seal(int shbuf_id);

        /// @brief shbuf set volatile
        int sys$shbuf_set_volatile(int shbuf_id, bool);

        /// @brief halt
        int sys$halt();

        /// @brief reboot
        int sys$reboot();

        /// @brief set proces icon
        int sys$set_process_icon(int icon_id);

        /// @brief realpath
        int sys$realpath(Userspace<const Syscall::SC_realpath_params*>);

        /// @brief getrandom
        ssize_t sys$getrandom(Userspace<void*>, size_t, unsigned int);

        /// @brief setkeymap
        int sys$setkeymap(Userspace<const Syscall::SC_setkeymap_params*>);

        /// @brief module load
        int sys$module_load(Userspace<const char*> path, size_t path_length);

        /// @brief module unload
        int sys$module_unload(Userspace<const char*> name, size_t name_length);

        /// @brief profiling enable
        int sys$profiling_enable(pid_t);

        /// @brief profiling disable
        int sys$profiling_disable(pid_t);

        /// @brief futex
        int sys$futex(Userspace<const Syscall::SC_futex_params*>);

        /// @brief set thread boost
        int sys$set_thread_boost(pid_t tid, int amount);

        /// @brief set process boost
        int sys$set_process_boost(pid_t, int amount);

        /// @brief chroot
        int sys$chroot(Userspace<const char*> path, size_t path_length, int mount_flags);

        /// @brief pledge
        int sys$pledge(Userspace<const Syscall::SC_pledge_params*>);

        /// @brief unveil
        int sys$unveil(Userspace<const Syscall::SC_unveil_params*>);

        /// @brief perf_event
        int sys$perf_event(int type, FlatPtr arg1, FlatPtr arg2);

        /// @brief get stack bounds
        int sys$get_stack_bounds(FlatPtr* stack_base, size_t* stack_size);

        /// @brief ptrace
        int sys$ptrace(Userspace<const Syscall::SC_ptrace_params*>);

        /// @brief sendfd
        int sys$sendfd(int sockfd, int fd);

        /// @brief recvfd
        int sys$recvfd(int sockfd);

        /// @brief sysconf
        long sys$sysconf(int name);

        /// @brief disown
        int sys$disown(ProcessID);

        /// @brief allocate_id
        void* sys$allocate_tls(size_t);

        /**
         * @tparam sockname 
         * @tparam Params 
         * @return int 
         */
        template<bool sockname, typename Params>
        int get_sock_or_peer_name(const Params&);

        static void initialize();

        /**
         * @param signal 
         * @param eip 
         * @param out_of_memory 
         */
        [[noreturn]] void crash(int signal, u32 eip, bool out_of_memory = false);

        static void reap(Process&);

        /**
         * @return siginfo_t 
         */
        [[nodiscard]] siginfo_t wait_info();

        /**
         * @return const TTY* 
         */
        const TTY* tty() const 
        { 
            return m_tty; 
        }

        void set_tty(TTY*);

        /**
         * @return size_t 
         */
        size_t region_count() const 
        { 
            return m_regions.size(); 
        }

        /**
         * @return const NonnullOwnPtrVector<Region>& 
         */
        const NonnullOwnPtrVector<Region>& regions() const
        {
            ASSERT(m_lock.is_locked());
            return m_regions;
        }

        void dump_regions();

        u32 m_ticks_in_user { 0 };
        u32 m_ticks_in_kernel { 0 };

        u32 m_ticks_in_user_for_dead_children { 0 };
        u32 m_ticks_in_kernel_for_dead_children { 0 };

        Custody& current_directory();

        /**
         * @return Custody* 
         */
        Custody* executable()
        {
            return m_executable.ptr();
        }

        int number_of_open_file_descriptors() const;

        /**
         * @return int 
         */
        int max_open_file_descriptors() const
        {
            return m_max_open_file_descriptors;
        }

        /**
         * @brief amount[clear + dirty, virtual, shared, purgeable]
         * 
         * @return size_t 
         */
        size_t amount_clean_inode() const;
        size_t amount_dirty_private() const;
        size_t amount_virtual() const;
        size_t amount_resident() const;
        size_t amount_shared() const;
        size_t amount_purgeable_volatile() const;
        size_t amount_purgeable_nonvolatile() const;

        /**
         * @param path 
         * @param arguments 
         * @param environment 
         * @param recusion_depth 
         * @return int 
         */
        int exec(String path, Vector<String> arguments, Vector<String> environment, int recusion_depth = 0);

        struct LoadResult 
        {
            FlatPtr load_base { 0 };
            FlatPtr entry_eip { 0 };
            size_t size { 0 };
            FlatPtr program_headers { 0 };
            size_t num_program_headers { 0 };
            Mods::WeakPtr<Region> tls_region;
            size_t tls_size { 0 };
            size_t tls_alignment { 0 };
        }; // struct LoadResult

        enum class ShouldAllocateTls 
        {
            No = 0,
            Yes,
        }; // enum class 

        /**
         * @param main_program_description 
         * @param interpreter_description 
         * @return int 
         */
        int load(NonnullRefPtr<FileDescription> main_program_description, RefPtr<FileDescription> interpreter_description);

        /**
         * @param object_description 
         * @param load_offset 
         * @return KResultOr<LoadResult> 
         */
        KResultOr<LoadResult> load_elf_object(FileDescription& object_description, FlatPtr load_offset, ShouldAllocateTls);

        /**
         * @return true 
         * @return false 
         */
        bool is_superuser() const
        {
            return m_euid == 0;
        }

        /**
         * @param offset_in_vmobject 
         * @param name 
         * @param prot 
         * @return Region* 
         */
        Region* allocate_region_with_vmobject(VirtualAddress, size_t, NonnullRefPtr<VMObject>, size_t offset_in_vmobject, const String& name, int prot);

        /**
         * @param name 
         * @param prot 
         * @param should_commit 
         * @return Region* 
         */
        Region* allocate_region(VirtualAddress, size_t, const String& name, int prot = PROT_READ | PROT_WRITE, bool should_commit = true);

        /**
         * @param offset_in_vmobject 
         * @param name 
         * @param prot 
         * @return Region* 
         */
        Region* allocate_region_with_vmobject(const Range&, NonnullRefPtr<VMObject>, size_t offset_in_vmobject, const String& name, int prot);

        /**
         * @param name 
         * @param prot 
         * @param should_commit 
         * @return Region* 
         */
        Region* allocate_region(const Range&, const String& name, int prot = PROT_READ | PROT_WRITE, bool should_commit = true);

        /**
         * @param region 
         * @return true 
         * @return false 
         */
        bool deallocate_region(Region& region);

        /**
         * @param source_region 
         * @param offset_in_vmobject 
         * @return Region& 
         */
        Region& allocate_split_region(const Region& source_region, const Range&, size_t offset_in_vmobject);

        /**
         * @param source_region 
         * @return Vector<Region*, 2> 
         */
        Vector<Region*, 2> split_region_around_range(const Region& source_region, const Range&);

        /**
         * @param signal 
         */
        void terminate_due_to_signal(u8 signal);

        /**
         * @param signal 
         * @param sender 
         * @return KResult 
         */
        KResult send_signal(u8 signal, Process* sender);

        /**
         * @return u16 
         */
        u16 thread_count() const
        {
            return m_thread_count.load(Mods::MemoryOrder::memory_order_relaxed);
        }

        /**
         * @return Lock& 
         */
        Lock& big_lock()
        {
            return m_big_lock;
        }

        struct ELFBundle 
        {
            OwnPtr<Region> region;
            RefPtr<ELF::Loader> elf_loader;
        }; // struct ELFBundle

        /**
         * @return OwnPtr<ELFBundle> 
         */
        OwnPtr<ELFBundle> elf_bundle() const;

        /**
         * @return int 
         */
        int icon_id() const
        {
            return m_icon_id;
        }

        /**
         * @return u32 
         */
        u32 priority_boost() const
        {
            return m_priority_boost;
        }

        /**
         * @return Custody& 
         */
        Custody& root_directory();

        /**
         * @return Custody& 
         */
        Custody& root_directory_relative_to_global_root();

        /// @brief: set root directory
        void set_root_directory(const Custody&);

        /**
         * @return true 
         * @return false 
         */
        bool has_promises() const
        {
            return m_promises;
        }

        /**
         * @param pledge 
         * @return true 
         * @return false 
         */
        bool has_promised(Pledge pledge) const
        {
            return m_promises & (1u << (u32)pledge);
        }

        /**
         * @return VeilState 
         */
        VeilState veil_state() const
        {
            return m_veil_state;
        }

        /**
         * @return const Vector<UnveiledPath>& 
         */
        const Vector<UnveiledPath>& unveiled_paths() const
        {
            return m_unveiled_paths;
        }

        /**
         * @return true 
         * @return false 
         */
        bool wait_for_tracer_at_next_execve() const
        {
            return m_wait_for_tracer_at_next_execve;
        }

        /**
         * @param val 
         */
        void set_wait_for_tracer_at_next_execve(bool val)
        {
            m_wait_for_tracer_at_next_execve = val;
        }

        /**
         * @param address 
         * @return KResultOr<u32> 
         */
        KResultOr<u32> peek_user_data(Userspace<const u32*> address);

        /**
         * @param address 
         * @param data 
         * @return KResult 
         */
        KResult poke_user_data(Userspace<u32*> address, u32 data);

        /**
         * @param process 
         */
        void disowned_by_waiter(Process& process);

        /**
         * @param signal 
         */
        void unblock_waiters(Thread::WaitBlocker::UnblockFlags, u8 signal = 0);

        /**
         * @return Thread::WaitBlockCondition& 
         */
        Thread::WaitBlockCondition& wait_block_condition() 
        { 
            return m_wait_block_condition; 
        }

    private:
        friend class MemoryManager;
        friend class Scheduler;
        friend class Region;

        /**
         * @param first_thread 
         * @param name 
         * @param ppid 
         * @param is_kernel_process 
         * @param cwd 
         * @param executable 
         * @param fork_parent 
         */
        Process(RefPtr<Thread>& first_thread, const String& name, uid_t, gid_t, ProcessID ppid, bool is_kernel_process, RefPtr<Custody> cwd = nullptr, RefPtr<Custody> executable = nullptr, TTY* = nullptr, Process* fork_parent = nullptr);

        /**
         * @return ProcessID 
         */
        static ProcessID allocate_pid();

        /**
         * @param alignment 
         * @return Range 
         */
        Range allocate_range(VirtualAddress, size_t, size_t alignment = PAGE_SIZE);

        /**
         * @return Region& 
         */
        Region& add_region(NonnullOwnPtr<Region>);

        /// @brief: kill threads
        void kill_threads_except_self();
        void kill_all_threads();

        /**
         * @param main_program_description 
         * @param arguments 
         * @param environment 
         * @param interpreter_description 
         * @param new_main_thread 
         * @param prev_flags 
         * @return int 
         */
        int do_exec(NonnullRefPtr<FileDescription> main_program_description, Vector<String> arguments, Vector<String> environment, RefPtr<FileDescription> interpreter_description, Thread*& new_main_thread, u32& prev_flags);

        /**
         * @return ssize_t 
         */
        ssize_t do_write(FileDescription&, const UserOrKernelBuffer&, size_t);

        /**
         * @param path 
         * @param nread 
         * @param file_size 
         * @return KResultOr<NonnullRefPtr<FileDescription>> 
         */
        KResultOr<NonnullRefPtr<FileDescription>> find_elf_interpreter_for_executable(const String& path, char (&first_page)[PAGE_SIZE], int nread, size_t file_size);

        /**
         * @return Vector<AuxiliaryValue> 
         */
        Vector<AuxiliaryValue> generate_auxiliary_vector() const;

        /**
         * @param first_candidate_fd 
         * @return int 
         */
        int alloc_fd(int first_candidate_fd = 0);

        /// @brief: disown all shred buffers.
        void disown_all_shared_buffers();

        /**
         * @param signal 
         * @return KResult 
         */
        KResult do_kill(Process&, int signal);

        /**
         * @param pgrp 
         * @param signal 
         * @return KResult 
         */
        KResult do_killpg(ProcessGroupID pgrp, int signal);

        /**
         * @param signal 
         * @return KResult 
         */
        KResult do_killall(int signal);

        /**
         * @param signal 
         * @return KResult 
         */
        KResult do_killself(int signal);

        /**
         * @param idtype 
         * @param id 
         * @param options 
         * @return KResultOr<siginfo_t> 
         */
        KResultOr<siginfo_t> do_waitid(idtype_t idtype, int id, int options);

        /**
         * @param user_path 
         * @param path_length 
         * @return KResultOr<String> 
         */
        KResultOr<String> get_syscall_path_argument(const char* user_path, size_t path_length) const;

        /**
         * @param user_path 
         * @param path_length 
         * @return KResultOr<String> 
         */
        KResultOr<String> get_syscall_path_argument(Userspace<const char*> user_path, size_t path_length) const
        {
            return get_syscall_path_argument(user_path.unsafe_userspace_ptr(), path_length);
        }

        /**
         * @return KResultOr<String> 
         */
        KResultOr<String> get_syscall_path_argument(const Syscall::StringArgument&) const;

        /**
         * @param tracer_pid 
         * @return true 
         * @return false 
         */
        bool has_tracee_thread(ProcessID tracer_pid);

        RefPtr<PageDirectory> m_page_directory;

        Process* m_prev { nullptr };
        Process* m_next { nullptr };

        String m_name;

        ProcessID m_pid { 0 };
        SessionID m_sid { 0 };
        RefPtr<ProcessGroup> m_pg;

        uid_t m_euid { 0 };
        gid_t m_egid { 0 };
        uid_t m_uid { 0 };
        gid_t m_gid { 0 };
        uid_t m_suid { 0 };
        gid_t m_sgid { 0 };

        ThreadID m_exec_tid { 0 };
        FlatPtr m_load_base { 0U };
        FlatPtr m_entry_eip { 0U };

        int m_main_program_fd { -1 };

        OwnPtr<ThreadTracer> m_tracer;

        static const int m_max_open_file_descriptors { FD_SETSIZE };

        class FileDescriptionAndFlags 
        {
        public:
            /**
             * @return true 
             * @return false 
             */
            operator bool() const 
            { 
                return !!m_description; 
            }

            /**
             * @return FileDescription* 
             */
            FileDescription* description() 
            { 
                return m_description; 
            }

            /**
             * @return const FileDescription* 
             */
            const FileDescription* description() const 
            { 
                return m_description; 
            }

            /**
             * @return u32 
             */
            u32 flags() const 
            { 
                return m_flags; 
            }
            
            /**
             * @param flags 
             */
            void set_flags(u32 flags) 
            { 
                m_flags = flags; 
            }

            void clear();

            /**
             * @param flags 
             */
            void set(NonnullRefPtr<FileDescription>&&, u32 flags = 0);

        private:
            RefPtr<FileDescription> m_description;
            u32 m_flags { 0 };
        }; // class FileDescriptionAndFlags

        Vector<FileDescriptionAndFlags> m_fds;

        u8 m_termination_status { 0 };
        u8 m_termination_signal { 0 };
        Atomic<u32> m_thread_count { 0 };

        const bool m_is_kernel_process;

        bool m_dead { false };
        bool m_profiling { false };
        Atomic<bool> m_is_stopped { false };
        bool m_should_dump_core { false };

        RefPtr<Custody> m_executable;
        RefPtr<Custody> m_cwd;
        RefPtr<Custody> m_root_directory;
        RefPtr<Custody> m_root_directory_relative_to_global_root;

        RefPtr<TTY> m_tty;

        Region* find_region_from_range(const Range&);
        Region* find_region_containing(const Range&);

        NonnullOwnPtrVector<Region> m_regions;
        struct RegionLookupCache 
        {
            Range range;
            WeakPtr<Region> region;
        };

        RegionLookupCache m_region_lookup_cache;

        ProcessID m_ppid { 0 };
        mode_t m_umask { 022 };

        Vector<gid_t> m_extra_gids;

        WeakPtr<Region> m_master_tls_region;
        size_t m_master_tls_size { 0 };
        size_t m_master_tls_alignment { 0 };

        Lock m_big_lock { "Process" };
        mutable SpinLock<u32> m_lock;

        RefPtr<Timer> m_alarm_timer;

        int m_icon_id { -1 };

        u32 m_priority_boost { 0 };

        u32 m_promises { 0 };
        u32 m_execpromises { 0 };

        VeilState m_veil_state { VeilState::None };

        Vector<UnveiledPath> m_unveiled_paths;

        WaitQueue& futex_queue(Userspace<const i32*>);

        HashMap<u32, OwnPtr<WaitQueue>> m_futex_queues;

        OwnPtr<PerformanceEventBuffer> m_perf_event_buffer;

        bool m_wait_for_tracer_at_next_execve { false };

        Thread::WaitBlockCondition m_wait_block_condition;
    };

    extern InlineLinkedList<Process>* g_processes;
    
    extern RecursiveSpinLock g_processes_lock;

    /**
     * @tparam Callback 
     * @param callback 
     */
    template<typename Callback>
    inline void Process::for_each(Callback callback)
    {
        ASSERT_INTERRUPTS_DISABLED();

        ScopedSpinLock lock(g_processes_lock);

        for (auto* process = g_processes->head(); process;) {
            auto* next_process = process->next();
            if (callback(*process) == IterationDecision::Break)
                break;
            process = next_process;
        }
    }

    /**
     * @tparam Callback 
     * @param callback 
     */
    template<typename Callback>
    inline void Process::for_each_child(Callback callback)
    {
        ASSERT_INTERRUPTS_DISABLED();

        ProcessID my_pid = pid();

        ScopedSpinLock lock(g_processes_lock);

        for (auto* process = g_processes->head(); process;) {
            auto* next_process = process->next();
            if (process->ppid() == my_pid || process->has_tracee_thread(m_pid)) {
                if (callback(*process) == IterationDecision::Break)
                    break;
            }
            process = next_process;
        }
    }
    
    /**
     * @tparam Callback 
     * @param callback 
     */
    template<typename Callback>
    inline void Process::for_each_thread(Callback callback) const
    {
        InterruptDisabler disabler;

        ProcessID my_pid = pid();

        if (my_pid == 0) {
            Processor::for_each(
                [&](Processor& proc) -> IterationDecision {
                    auto idle_thread = proc.idle_thread();
                    if (idle_thread != nullptr)
                        return callback(*idle_thread);
                    return IterationDecision::Continue;
                });
            return;
        }

        Thread::for_each([callback, my_pid](Thread& thread) -> IterationDecision {
            if (thread.pid() == my_pid)
                return callback(thread);

            return IterationDecision::Continue;
        });
    }

    /**
     * @tparam Callback 
     * @param pgid 
     * @param callback 
     */
    template<typename Callback>
    inline void Process::for_each_in_pgrp(ProcessGroupID pgid, Callback callback)
    {
        ASSERT_INTERRUPTS_DISABLED();

        ScopedSpinLock lock(g_processes_lock);

        for (auto* process = g_processes->head(); process;) {
            auto* next_process = process->next();
            if (!process->is_dead() && process->pgid() == pgid) {
                if (callback(*process) == IterationDecision::Break)
                    break;
            }
            process = next_process;
        }
    }

    /**
     * @param process 
     * @return true 
     * @return false 
     */
    inline bool InodeMetadata::may_read(const Process& process) const
    {
        return may_read(process.euid(), process.egid(), process.extra_gids());
    }
    
    /**
     * @param process 
     * @return true 
     * @return false 
     */
    inline bool InodeMetadata::may_write(const Process& process) const
    {
        return may_write(process.euid(), process.egid(), process.extra_gids());
    }

    /**
     * @param process 
     * @return true 
     * @return false 
     */
    inline bool InodeMetadata::may_execute(const Process& process) const
    {
        return may_execute(process.euid(), process.egid(), process.extra_gids());
    }       

    /**
     * @return ProcessID 
     */
    inline ProcessID Thread::pid() const
    {
        return m_process->pid();
    }

    /**
     * @param stream 
     * @param process 
     * @return const LogStream& 
     */
    inline const LogStream& operator<<(const LogStream& stream, const Process& process)
    {
        return stream << process.name() << '(' << process.pid().value() << ')';
    }

    /**
     * @return u32 
     */
    inline u32 Thread::effective_priority() const
    {
        return m_priority + m_process->priority_boost() + m_priority_boost + m_extra_priority;
    }

    #define REQUIRE_NO_PROMISES                        \
        do {                                           \
            if (Process::current()->has_promises()) {  \
                dbg() << "Has made a promise";         \
                cli();                                 \
                Process::current()->crash(SIGABRT, 0); \
                ASSERT_NOT_REACHED();                  \
            }                                          \
        } while (0)

    #define REQUIRE_PROMISE(promise)                                     \
        do {                                                             \
            if (Process::current()->has_promises()                       \
                && !Process::current()->has_promised(Pledge::promise)) { \
                dbg() << "Has not pledged " << #promise;                 \
                cli();                                                   \
                Process::current()->crash(SIGABRT, 0);                   \
                ASSERT_NOT_REACHED();                                    \
            }                                                            \
        } while (0)

}

/**
 * @param string 
 * @return String 
 */
inline static String copy_string_from_user(const Kernel::Syscall::StringArgument& string)
{
    return copy_string_from_user(string.characters, string.length);
}
