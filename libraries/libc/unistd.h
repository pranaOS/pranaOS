/**
 * @file unistd.h
 * @author Krisna Pranav
 * @brief unistd
 * @version 6.0
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <errno.h>
#include <limits.h>
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

#define HZ 1000
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#ifndef _STDIO_H      
#    define SEEK_SET 0
#    define SEEK_CUR 1
#    define SEEK_END 2
#endif

extern char** environ;

/**
 * @param buffer 
 * @param buffer_size 
 * @return int 
 */
int get_process_name(char* buffer, int buffer_size);

/**
 * @param name 
 * @param name_length 
 * @return int 
 */
int set_process_name(const char* name, size_t name_length);

/// @brief: dump_backtrace
void dump_backtrace();

/**
 * @param fd 
 * @return int 
 */
int fsync(int fd);

/// @brief: sysbeep
void sysbeep();

/**
 * @return int 
 */
int gettid();

/**
 * @param tid 
 * @return int 
 */
int donate(int tid);

/**
 * @param icon_id 
 * @return int 
 */
int set_process_icon(int icon_id);

/**
 * @return int 
 */
int getpagesize();

/**
 * @return pid_t 
 */
pid_t fork();

/**
 * @param path 
 * @param argv 
 * @return int 
 */
int execv(const char* path, char* const argv[]);

/**
 * @param filename 
 * @param argv 
 * @param envp 
 * @return int 
 */
int execve(const char* filename, char* const argv[], char* const envp[]);

/**
 * @param filename 
 * @param argv 
 * @param envp 
 * @return int 
 */
int execvpe(const char* filename, char* const argv[], char* const envp[]);

/**
 * @param filename 
 * @param argv 
 * @return int 
 */
int execvp(const char* filename, char* const argv[]);

/**
 * @param filename 
 * @param arg 
 * @param ... 
 * @return int 
 */
int execl(const char* filename, const char* arg, ...);

/**
 * @param filename 
 * @param arg 
 * @param ... 
 * @return int 
 */
int execlp(const char* filename, const char* arg, ...);

/**
 * @param path 
 * @return int 
 */
int chroot(const char* path);

/**
 * @param path 
 * @param mount_flags 
 * @return int 
 */
int chroot_with_mount_flags(const char* path, int mount_flags);

/// @brief: sync
void sync();

/// @brief: exit
__attribute__((noreturn)) void _exit(int status);

/**
 * @return pid_t 
 */
pid_t getsid(pid_t);

/// @return pid_t 
pid_t setsid();

/**
 * @param pid 
 * @param pgid 
 * @return int 
 */
int setpgid(pid_t pid, pid_t pgid);

/// @return pid_t
pid_t getpgid(pid_t);

/// @return pid_t 
pid_t getpgrp();

/// @return uid_t 
uid_t geteuid();

/// @return gid_t 
gid_t getegid();

/// @return uid_t 
uid_t getuid();

/// @return gid_t 
gid_t getgid();

/// @return pid_t 
pid_t getpid();

/// @return pid_t 
pid_t getppid();

/**
 * @return int 
 */
int getresuid(uid_t*, uid_t*, uid_t*);

/**
 * @return int 
 */
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
int setgroups(size_t, const gid_t*);

/**
 * @return int 
 */
int seteuid(uid_t);

/// @return int
int setegid(gid_t);

/// @return int
int setuid(uid_t);

/// @return int
int setgid(gid_t);

/// @return int
int setresuid(uid_t, uid_t, uid_t);

/**
 * @return int 
 */
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
ssize_t write(int fd, const void* buf, size_t count);

/**
 * @param fd 
 * @return int 
 */
int close(int fd);

/**
 * @param path 
 * @return int 
 */
int chdir(const char* path);

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
 * @return int 
 */
int sleep(unsigned seconds);

/**
 * @return int 
 */
int usleep(useconds_t);

/**
 * @return int 
 */
int gethostname(char*, size_t);

/**
 * @return int 
 */
int sethostname(const char*, ssize_t);

/**
 * @param path 
 * @param buffer 
 * @return ssize_t 
 */
ssize_t readlink(const char* path, char* buffer, size_t);

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
int link(const char* oldpath, const char* newpath);

/**
 * @param pathname 
 * @return int 
 */
int unlink(const char* pathname);

/**
 * @param target 
 * @param linkpath 
 * @return int 
 */
int symlink(const char* target, const char* linkpath);

/**
 * @param pathname 
 * @return int 
 */
int rmdir(const char* pathname);

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
int access(const char* pathname, int mode);

/**
 * @param fd 
 * @return int 
 */
int isatty(int fd);

/**
 * @param pathname 
 * @return int 
 */
int mknod(const char* pathname, mode_t, dev_t);

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
long pathconf(const char* path, int name);

/**
 * @return char* 
 */
char* getlogin();

/**
 * @param pathname 
 * @return int 
 */
int chown(const char* pathname, uid_t, gid_t);

/**
 * @param fd 
 * @return int 
 */
int fchown(int fd, uid_t, gid_t);

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
int truncate(const char* path, off_t length);

/**
 * @return int 
 */
int halt();

/**
 * @return int 
 */
int reboot();

/**
 * @param source_fd 
 * @param target 
 * @param fs_type 
 * @param flags 
 * @return int 
 */
int mount(int source_fd, const char* target, const char* fs_type, int flags);

/**
 * @param mountpoint 
 * @return int 
 */
int umount(const char* mountpoint);

/**
 * @param promises 
 * @param execpromises 
 * @return int 
 */
int pledge(const char* promises, const char* execpromises);

/**
 * @param path 
 * @param permissions 
 * @return int 
 */
int unveil(const char* path, const char* permissions);

/**
 * @param prompt 
 * @return char* 
 */
char* getpass(const char* prompt);

enum 
{
    _PC_NAME_MAX,
    _PC_PATH_MAX,
    _PC_PIPE_BUF,
    _PC_VDISABLE
}; // enum 

#define HOST_NAME_MAX 64

#define R_OK 4
#define W_OK 2
#define X_OK 1
#define F_OK 0

#define MS_NODEV (1 << 0)
#define MS_NOEXEC (1 << 1)
#define MS_NOSUID (1 << 2)
#define MS_BIND (1 << 3)
#define MS_RDONLY (1 << 4)
#define MS_REMOUNT (1 << 5)

#define _POSIX_SAVED_IDS
#define _POSIX_PRIORITY_SCHEDULING
#define _POSIX_VDISABLE '\0'

enum 
{
    _SC_NPROCESSORS_CONF,
    _SC_NPROCESSORS_ONLN,
    _SC_PAGESIZE,
    _SC_OPEN_MAX,
}; // enum

/// @breif: _SC
#define _SC_NPROCESSORS_CONF _SC_NPROCESSORS_CONF
#define _SC_NPROCESSORS_ONLN _SC_NPROCESSORS_ONLN
#define _SC_PAGESIZE _SC_PAGESIZE
#define _SC_OPEN_MAX _SC_OPEN_MAX

/**
 * @param name 
 * @return long 
 */
long sysconf(int name);

/// @brief: crypto_data
struct crypt_data 
{
    int initialized;
    char result[65];
};

/**
 * @param key 
 * @param salt 
 * @return char* 
 */
char* crypt(const char* key, const char* salt);

/**
 * @param key 
 * @param salt 
 * @param data 
 * @return char* 
 */
char* crypt_r(const char* key, const char* salt, struct crypt_data* data);


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
int getopt(int argc, char** argv, const char* short_options);

__END_DECLS
