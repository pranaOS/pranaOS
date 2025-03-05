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
ErrorOr<void> profiling_disable(pid_t);
ErrorOr<void> profiling_free_buffer(pid_t);
#endif

#ifndef MODS_OS_BSD_GENERIC

/**
 * @return ErrorOr<Optional<struct spwd>> 
 */
ErrorOr<Optional<struct spwd>> getspent();

/**
 * @param name 
 * @return ErrorOr<Optional<struct spwd>> 
 */
ErrorOr<Optional<struct spwd>> getspnam(StringView name);
#endif

#ifndef MODS_OS_MACOS
/**
 * @param sockfd 
 * @param flags 
 * @return ErrorOr<int> 
 */
ErrorOr<int> accept4(int sockfd, struct sockaddr*, socklen_t*, int flags);
#endif

/**
 * @param signal 
 * @param action 
 * @param old_action 
 * @return ErrorOr<void> 
 */
ErrorOr<void> sigaction(int signal, struct sigaction const* action, struct sigaction* old_action);
#if defined(__APPLE__) || defined(__OpenBSD__) || defined(__FreeBSD__)
ErrorOr<sig_t> signal(int signal, sig_t handler);
#else
ErrorOr<sighandler_t> signal(int signal, sighandler_t handler);
#endif

/**
 * @param fd 
 * @return ErrorOr<struct stat> 
 */
ErrorOr<struct stat> fstat(int fd);

/**
 * @param fd 
 * @param command 
 * @param ... 
 * @return ErrorOr<int> 
 */
ErrorOr<int> fcntl(int fd, int command, ...);

/**
 * @param address 
 * @param protection 
 * @param flags 
 * @param fd 
 * @param alignment 
 * @param name 
 * @return ErrorOr<void*> 
 */
ErrorOr<void*> mmap(void* address, size_t, int protection, int flags, int fd, off_t, size_t alignment = 0, StringView name = {});

/**
 * @param address 
 * @return ErrorOr<void> 
 */
ErrorOr<void> munmap(void* address, size_t);

/**
 * @param size 
 * @param options 
 * @return ErrorOr<int> 
 */
ErrorOr<int> anon_create(size_t size, int options);

/**
 * @param path 
 * @param options 
 * @param mode 
 * @return ErrorOr<int> 
 */
ErrorOr<int> open(StringView path, int options, mode_t mode = 0);

/**
 * @param fd 
 * @param path 
 * @param options 
 * @param mode 
 * @return ErrorOr<int> 
 */
ErrorOr<int> openat(int fd, StringView path, int options, mode_t mode = 0);

/**
 * @param fd 
 * @return ErrorOr<void> 
 */
ErrorOr<void> close(int fd);

/**
 * @param fd 
 * @param length 
 * @return ErrorOr<void> 
 */
ErrorOr<void> ftruncate(int fd, off_t length);

/**
 * @param path 
 * @return ErrorOr<struct stat> 
 */
ErrorOr<struct stat> stat(StringView path);

/**
 * @param path 
 * @return ErrorOr<struct stat> 
 */
ErrorOr<struct stat> lstat(StringView path);

/**
 * @param fd 
 * @param buffer 
 * @return ErrorOr<ssize_t> 
 */
ErrorOr<ssize_t> read(int fd, Bytes buffer);

/**
 * @param fd 
 * @param buffer 
 * @return ErrorOr<ssize_t> 
 */
ErrorOr<ssize_t> write(int fd, ReadonlyBytes buffer);

/**
 * @param signal 
 * @return ErrorOr<void> 
 */
ErrorOr<void> kill(pid_t, int signal);

/**
 * @param pgrp 
 * @param signal 
 * @return ErrorOr<void> 
 */
ErrorOr<void> killpg(int pgrp, int signal);

/**
 * @param source_fd 
 * @return ErrorOr<int> 
 */
ErrorOr<int> dup(int source_fd);

/**
 * @param source_fd 
 * @param destination_fd 
 * @return ErrorOr<int> 
 */
ErrorOr<int> dup2(int source_fd, int destination_fd);

/**
 * @param fd 
 * @return ErrorOr<String> 
 */
ErrorOr<String> ptsname(int fd);

ErrorOr<String> gethostname();

ErrorOr<void> sethostname(StringView);

ErrorOr<String> getcwd();

/**
 * @param fd 
 * @param request 
 * @param ... 
 * @return ErrorOr<void> 
 */
ErrorOr<void> ioctl(int fd, unsigned request, ...);

/**
 * @param fd 
 * @return ErrorOr<struct termios> 
 */
ErrorOr<struct termios> tcgetattr(int fd);

/**
 * @param fd 
 * @param optional_actions 
 * @return ErrorOr<void> 
 */
ErrorOr<void> tcsetattr(int fd, int optional_actions, struct termios const&);

/**
 * @param fd 
 * @param pgrp 
 * @return ErrorOr<int> 
 */
ErrorOr<int> tcsetpgrp(int fd, pid_t pgrp);

/**
 * @param pathname 
 * @param mode 
 * @return ErrorOr<void> 
 */
ErrorOr<void> chmod(StringView pathname, mode_t mode);

/**
 * @param pathname 
 * @param uid 
 * @param gid 
 * @return ErrorOr<void> 
 */
ErrorOr<void> lchown(StringView pathname, uid_t uid, gid_t gid);

/**
 * @param pathname 
 * @param uid 
 * @param gid 
 * @return ErrorOr<void> 
 */
ErrorOr<void> chown(StringView pathname, uid_t uid, gid_t gid);

/**
 * @param name 
 * @return ErrorOr<Optional<struct passwd>> 
 */
ErrorOr<Optional<struct passwd>> getpwnam(StringView name);
ErrorOr<Optional<struct group>> getgrnam(StringView name);

ErrorOr<Optional<struct passwd>> getpwuid(uid_t);
ErrorOr<Optional<struct group>> getgrgid(gid_t);

/**
 * @param clock_id 
 * @param ts 
 * @return ErrorOr<void> 
 */
ErrorOr<void> clock_settime(clockid_t clock_id, struct timespec* ts);

/**
 * @param path 
 * @param file_actions 
 * @param attr 
 * @param arguments 
 * @param envp 
 * @return ErrorOr<pid_t> 
 */
ErrorOr<pid_t> posix_spawnp(StringView const path, posix_spawn_file_actions_t* const file_actions, posix_spawnattr_t* const attr, char* const arguments[], char* const envp[]);

/**
 * @param fd 
 * @param whence 
 * @return ErrorOr<off_t> 
 */
ErrorOr<off_t> lseek(int fd, off_t, int whence);

struct WaitPidResult {
    pid_t pid;
    int status;
}; // struct WaitPidResult

/**
 * @param waitee 
 * @param options 
 * @return ErrorOr<WaitPidResult> 
 */
ErrorOr<WaitPidResult> waitpid(pid_t waitee, int options = 0);

ErrorOr<void> setuid(uid_t);
ErrorOr<void> seteuid(uid_t);
ErrorOr<void> setgid(gid_t);
ErrorOr<void> setegid(gid_t);

/**
 * @param pid 
 * @param pgid 
 * @return ErrorOr<void> 
 */
ErrorOr<void> setpgid(pid_t pid, pid_t pgid);

ErrorOr<pid_t> setsid();
ErrorOr<void> drop_privileges();

/**
 * @param fd 
 * @return ErrorOr<bool> 
 */
ErrorOr<bool> isatty(int fd);

/**
 * @param target 
 * @param link_path 
 * @return ErrorOr<void> 
 */
ErrorOr<void> symlink(StringView target, StringView link_path);

/**
 * @param path 
 * @return ErrorOr<void> 
 */
ErrorOr<void> mkdir(StringView path, mode_t);

/**
 * @param path 
 * @return ErrorOr<void> 
 */
ErrorOr<void> chdir(StringView path);

/**
 * @param path 
 * @return ErrorOr<void> 
 */
ErrorOr<void> rmdir(StringView path);

ErrorOr<pid_t> fork();

/**
 * @param pattern 
 * @return ErrorOr<int> 
 */
ErrorOr<int> mkstemp(Span<char> pattern);

/**
 * @param fd 
 * @param mode 
 * @return ErrorOr<void> 
 */
ErrorOr<void> fchmod(int fd, mode_t mode);

/**
 * @param fd 
 * @return ErrorOr<void> 
 */
ErrorOr<void> fchown(int fd, uid_t, gid_t);

/**
 * @param old_path 
 * @param new_path 
 * @return ErrorOr<void> 
 */
ErrorOr<void> rename(StringView old_path, StringView new_path);

/**
 * @param path 
 * @return ErrorOr<void> 
 */
ErrorOr<void> unlink(StringView path);

/**
 * @param path 
 * @return ErrorOr<void> 
 */
ErrorOr<void> utime(StringView path, Optional<struct utimbuf>);

ErrorOr<struct utsname> uname();

/**
 * @param flags 
 * @return ErrorOr<Array<int, 2>> 
 */
ErrorOr<Array<int, 2>> pipe2(int flags);

/**
 * @param delta 
 * @param old_delta 
 * @return ErrorOr<void> 
 */
ErrorOr<void> adjtime(const struct timeval* delta, struct timeval* old_delta);

enum class SearchInPath {
    No,
    Yes,
}; // enum class SearchInPath

/**
 * @param filename 
 * @param arguments 
 * @param environment 
 * @return ErrorOr<void> 
 */
ErrorOr<void> exec(StringView filename, Span<StringView> arguments, SearchInPath, Optional<Span<StringView>> environment = {});

/**
 * @param domain 
 * @param type 
 * @param protocol 
 * @return ErrorOr<int> 
 */
ErrorOr<int> socket(int domain, int type, int protocol);

/**
 * @param sockfd 
 * @return ErrorOr<void> 
 */
ErrorOr<void> bind(int sockfd, struct sockaddr const*, socklen_t);

/**
 * @param sockfd 
 * @param backlog 
 * @return ErrorOr<void> 
 */
ErrorOr<void> listen(int sockfd, int backlog);

/**
 * @param sockfd 
 * @return ErrorOr<int> 
 */
ErrorOr<int> accept(int sockfd, struct sockaddr*, socklen_t*);

/**
 * @param sockfd 
 * @return ErrorOr<void> 
 */
ErrorOr<void> connect(int sockfd, struct sockaddr const*, socklen_t);

/**
 * @param sockfd 
 * @param how 
 * @return ErrorOr<void> 
 */
ErrorOr<void> shutdown(int sockfd, int how);

/**
 * @param sockfd 
 * @param flags 
 * @return ErrorOr<ssize_t> 
 */
ErrorOr<ssize_t> send(int sockfd, void const*, size_t, int flags);

/**
 * @param sockfd 
 * @param flags 
 * @return ErrorOr<ssize_t> 
 */
ErrorOr<ssize_t> sendmsg(int sockfd, const struct msghdr*, int flags);

/**
 * @param sockfd 
 * @param flags 
 * @return ErrorOr<ssize_t> 
 */
ErrorOr<ssize_t> sendto(int sockfd, void const*, size_t, int flags, struct sockaddr const*, socklen_t);

/**
 * @param sockfd 
 * @param flags 
 * @return ErrorOr<ssize_t> 
 */
ErrorOr<ssize_t> recv(int sockfd, void*, size_t, int flags);

/**
 * @param sockfd 
 * @param flags 
 * @return ErrorOr<ssize_t> 
 */
ErrorOr<ssize_t> recvmsg(int sockfd, struct msghdr*, int flags);

/**
 * @param sockfd 
 * @param flags 
 * @return ErrorOr<ssize_t> 
 */
ErrorOr<ssize_t> recvfrom(int sockfd, void*, size_t, int flags, struct sockaddr*, socklen_t*);

/**
 * @param sockfd 
 * @param level 
 * @param option 
 * @param value 
 * @param value_size 
 * @return ErrorOr<void> 
 */
ErrorOr<void> getsockopt(int sockfd, int level, int option, void* value, socklen_t* value_size);

/**
 * @param sockfd 
 * @param level 
 * @param option 
 * @param value 
 * @param value_size 
 * @return ErrorOr<void> 
 */
ErrorOr<void> setsockopt(int sockfd, int level, int option, void const* value, socklen_t value_size);

/**
 * @param sockfd 
 * @return ErrorOr<void> 
 */
ErrorOr<void> getsockname(int sockfd, struct sockaddr*, socklen_t*);

/**
 * @param sockfd 
 * @return ErrorOr<void> 
 */
ErrorOr<void> getpeername(int sockfd, struct sockaddr*, socklen_t*);

/**
 * @param domain 
 * @param type 
 * @param protocol 
 * @param sv 
 * @return ErrorOr<void> 
 */
ErrorOr<void> socketpair(int domain, int type, int protocol, int sv[2]);

ErrorOr<Vector<gid_t>> getgroups();

/**
 * @param pathname 
 * @param mode 
 * @param dev 
 * @return ErrorOr<void> 
 */
ErrorOr<void> mknod(StringView pathname, mode_t mode, dev_t dev);

/**
 * @param pathname 
 * @param mode 
 * @return ErrorOr<void> 
 */
ErrorOr<void> mkfifo(StringView pathname, mode_t mode);

/**
 * @return ErrorOr<void> 
 */
ErrorOr<void> setenv(StringView, StringView, bool);

/**
 * @param flags 
 * @return ErrorOr<int> 
 */
ErrorOr<int> posix_openpt(int flags);

/**
 * @param fildes 
 * @return ErrorOr<void> 
 */
ErrorOr<void> grantpt(int fildes);

/**
 * @param fildes 
 * @return ErrorOr<void> 
 */
ErrorOr<void> unlockpt(int fildes);

/**
 * @param pathname 
 * @param mode 
 * @return ErrorOr<void> 
 */
ErrorOr<void> access(StringView pathname, int mode);

} // namespace Core::System
