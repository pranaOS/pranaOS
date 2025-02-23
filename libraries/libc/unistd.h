/**
 * @file unistd.h
 * @author Krisna Pranav
 * @brief unistd
 * @version 6.0
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <Kernel/API/POSIX/unistd.h>
#include <fd_set.h>
#include <limits.h>

__BEGIN_DECLS

#define HZ 1000

#ifndef _STDIO_H       
#    define SEEK_SET 0 
#    define SEEK_CUR 1 
#    define SEEK_END 2 
#endif

extern char** environ;

/**
 * @brief Get the process name object
 * 
 * @param buffer 
 * @param buffer_size 
 * @return int 
 */
int get_process_name(char* buffer, int buffer_size);

/**
 * @brief Set the process name object
 * 
 * @param name 
 * @param name_length 
 * @return int 
 */
int set_process_name(char const* name, size_t name_length);

void dump_backtrace(void);

/**
 * @param fd 
 * @return int 
 */
int fsync(int fd);
int sysbeep(void);
int gettid(void);
int getpagesize(void);

/**
 * @return pid_t 
 */
pid_t fork(void);

/**
 * @return pid_t 
 */
pid_t vfork(void);

/**
 * @param nochdir 
 * @param noclose 
 * @return int 
 */
int daemon(int nochdir, int noclose);

/**
 * @param path 
 * @param argv 
 * @return int 
 */
int execv(char const* path, char* const argv[]);

/**
 * @param filename 
 * @param argv 
 * @param envp 
 * @return int 
 */
int execve(char const* filename, char* const argv[], char* const envp[]);

/**
 * @param filename 
 * @param argv 
 * @param envp 
 * @return int 
 */
int execvpe(char const* filename, char* const argv[], char* const envp[]);

/**
 * @param filename 
 * @param argv 
 * @return int 
 */
int execvp(char const* filename, char* const argv[]);

/**
 * @param filename 
 * @param arg 
 * @param ... 
 * @return int 
 */
int execl(char const* filename, char const* arg, ...);

/**
 * @param filename 
 * @param arg 
 * @param ... 
 * @return int 
 */
int execle(char const* filename, char const* arg, ...);

/**
 * @param filename 
 * @param arg 
 * @param ... 
 * @return int 
 */
int execlp(char const* filename, char const* arg, ...);

void sync(void);

/**
 * @param status
 */
__attribute__((noreturn)) void _exit(int status);

/**
 * @return pid_t 
 */
pid_t getsid(pid_t);
pid_t setsid(void);

/**
 * @param pid 
 * @param pgid 
 * @return int 
 */
int setpgid(pid_t pid, pid_t pgid);

/**
 * @return pid_t 
 */
pid_t getpgid(pid_t);
pid_t getpgrp(void);

/**
 * @return uid_t 
 */
uid_t geteuid(void);
gid_t getegid(void);
uid_t getuid(void);
gid_t getgid(void);

/**
 * @return pid_t 
 */
pid_t getpid(void);
pid_t getppid(void);

/**
 * @return int 
 */
int getresuid(uid_t*, uid_t*, uid_t*);
int getresgid(gid_t*, gid_t*, gid_t*);

/**
 * @param size 
 * @param list 
 * @return int 
 */
int getgroups(int size, gid_t list[]);

/**
 * @return int 
 */
int setgroups(size_t, gid_t const*);
int seteuid(uid_t);
int setegid(gid_t);
int setuid(uid_t);
int setgid(gid_t);
int setreuid(uid_t, uid_t);
int setresuid(uid_t, uid_t, uid_t);
int setresgid(gid_t, gid_t, gid_t);

/**
 * @param fd 
 * @return pid_t 
 */
pid_t tcgetpgrp(int fd);

/**
 * @param fd 
 * @param pgid 
 * @return int 
 */
int tcsetpgrp(int fd, pid_t pgid);

/**
 * @param fd 
 * @param buf 
 * @param count 
 * @return ssize_t 
 */
ssize_t read(int fd, void* buf, size_t count);

/**
 * @param fd 
 * @param buf 
 * @param count 
 * @return ssize_t 
 */
ssize_t pread(int fd, void* buf, size_t count, off_t);

/**
 * @param fd 
 * @param buf 
 * @param count 
 * @return ssize_t 
 */
ssize_t write(int fd, void const* buf, size_t count);

/**
 * @param fd 
 * @param buf 
 * @param count 
 * @return ssize_t 
 */
ssize_t pwrite(int fd, void const* buf, size_t count, off_t);

/**
 * @param fd 
 * @return int 
 */
int close(int fd);

/**
 * @param path 
 * @return int 
 */
int chdir(char const* path);

/**
 * @param fd 
 * @return int 
 */
int fchdir(int fd);

/**
 * @param buffer 
 * @param size 
 * @return char* 
 */
char* getcwd(char* buffer, size_t size);

/**
 * @param buffer 
 * @return char* 
 */
char* getwd(char* buffer);

/**
 * @param seconds 
 * @return unsigned int 
 */
unsigned int sleep(unsigned int seconds);

int usleep(useconds_t);
int gethostname(char*, size_t);
int sethostname(char const*, ssize_t);

/**
 * @param path 
 * @param buffer 
 * @return ssize_t 
 */
ssize_t readlink(char const* path, char* buffer, size_t);

/**
 * @param fd 
 * @return char* 
 */
char* ttyname(int fd);

/**
 * @param fd 
 * @param buffer 
 * @return int 
 */
int ttyname_r(int fd, char* buffer, size_t);

/**
 * @param fd 
 * @param whence 
 * @return off_t 
 */
off_t lseek(int fd, off_t, int whence);

/**
 * @param oldpath 
 * @param newpath 
 * @return int 
 */
int link(char const* oldpath, char const* newpath);

/**
 * @param pathname 
 * @return int 
 */
int unlink(char const* pathname);

/**
 * @param dirfd 
 * @param pathname 
 * @param flags 
 * @return int 
 */
int unlinkat(int dirfd, char const* pathname, int flags);

/**
 * @param target 
 * @param linkpath 
 * @return int 
 */
int symlink(char const* target, char const* linkpath);

/**
 * @param pathname 
 * @return int 
 */
int rmdir(char const* pathname);

/**
 * @param old_fd 
 * @return int 
 */
int dup(int old_fd);

/**
 * @param old_fd 
 * @param new_fd 
 * @return int 
 */
int dup2(int old_fd, int new_fd);

/**
 * @param pipefd 
 * @return int 
 */
int pipe(int pipefd[2]);

/**
 * @param pipefd 
 * @param flags 
 * @return int 
 */
int pipe2(int pipefd[2], int flags);

/**
 * @param seconds 
 * @return unsigned int 
 */
unsigned int alarm(unsigned int seconds);

/**
 * @param pathname 
 * @param mode 
 * @return int 
 */
int access(char const* pathname, int mode);

/**
 * @param fd 
 * @return int 
 */
int isatty(int fd);

/**
 * @param pathname 
 * @return int 
 */
int mknod(char const* pathname, mode_t, dev_t);

/**
 * @param fd 
 * @param name 
 * @return long 
 */
long fpathconf(int fd, int name);

/**
 * @param path 
 * @param name 
 * @return long 
 */
long pathconf(char const* path, int name);

/**
 * @return char* 
 */
char* getlogin(void);

/**
 * @param pathname 
 * @param uid 
 * @param gid 
 * @return int 
 */
int lchown(char const* pathname, uid_t uid, gid_t gid);

/**
 * @param pathname 
 * @return int 
 */
int chown(char const* pathname, uid_t, gid_t);

/**
 * @param fd 
 * @return int 
 */
int fchown(int fd, uid_t, gid_t);

/**
 * @param fd 
 * @param pathname 
 * @param uid 
 * @param gid 
 * @param flags 
 * @return int 
 */
int fchownat(int fd, char const* pathname, uid_t uid, gid_t gid, int flags);

/**
 * @param fd 
 * @param length 
 * @return int 
 */
int ftruncate(int fd, off_t length);

/**
 * @param path 
 * @param length 
 * @return int 
 */
int truncate(char const* path, off_t length);

/**
 * @param source_fd 
 * @param target 
 * @param fs_type 
 * @param flags 
 * @return int 
 */
int mount(int source_fd, char const* target, char const* fs_type, int flags);

/**
 * @param mountpoint 
 * @return int 
 */
int umount(char const* mountpoint);

/**
 * @param promises 
 * @param execpromises 
 * @return int 
 */
int pledge(char const* promises, char const* execpromises);

/**
 * @param path 
 * @param permissions 
 * @return int 
 */
int unveil(char const* path, char const* permissions);

/**
 * @param prompt 
 * @return char* 
 */
char* getpass(char const* prompt);

int pause(void);
int chroot(char const*);
int getdtablesize(void);

enum {
    _PC_NAME_MAX,
    _PC_PATH_MAX,
    _PC_PIPE_BUF,
    _PC_VDISABLE,
    _PC_LINK_MAX
}; // enum

#define _POSIX_FSYNC 200112L
#define _POSIX_MAPPED_FILES 200112L
#define _POSIX_MEMORY_PROTECTION 200112L
#define _POSIX_MONOTONIC_CLOCK 200112L
#define _POSIX_RAW_SOCKETS 200112L
#define _POSIX_REGEXP 1
#define _POSIX_SAVED_IDS 1
#define _POSIX_SPAWN 200112L
#define _POSIX_THREADS 200112L
#define _POSIX_THREAD_ATTR_STACKADDR 200112L
#define _POSIX_THREAD_ATTR_STACKSIZE 200112L
#define _POSIX_TIMERS 200809L

#define _POSIX_PRIORITY_SCHEDULING
#define _POSIX_VDISABLE '\0'

/**
 * @param name 
 * @return long 
 */
long sysconf(int name);

extern int opterr;
extern int optopt;
extern int optind;
extern int optreset;
extern char* optarg;

/**
 * @param argc 
 * @param argv 
 * @param short_options 
 * @return int 
 */
int getopt(int argc, char* const* argv, char const* short_options);

__END_DECLS
