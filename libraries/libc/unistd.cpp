/**
 * @file unistd.cpp
 * @author Krisna Pranav
 * @brief unistd
 * @version 6.0
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */


#include <mods/scopeguard.h>
#include <mods/scopedvaluerollback.h>
#include <mods/string.h>
#include <mods/vector.h>
#include <alloca.h>
#include <assert.h>
#include <bits/pthread_integration.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <grp.h>
#include <pwd.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <sys/types.h>
#include <syscall.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

extern "C" {

#ifdef NO_TLS
static int s_cached_tid = 0;
#else
static __thread int s_cached_tid = 0;
#endif

static int s_cached_pid = 0;

/**
 * @param pathname 
 * @param uid 
 * @param gid 
 * @return int 
 */
int lchown(char const* pathname, uid_t uid, gid_t gid)
{
    if (!pathname) {
        errno = EFAULT;
        return -1;
    }
    Syscall::SC_chown_params params { { pathname, strlen(pathname) }, uid, gid, AT_FDCWD, false };
    int rc = syscall(SC_chown, &params);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param pathname 
 * @param uid 
 * @param gid 
 * @return int 
 */
int chown(char const* pathname, uid_t uid, gid_t gid)
{
    if (!pathname) {
        errno = EFAULT;
        return -1;
    }
    Syscall::SC_chown_params params { { pathname, strlen(pathname) }, uid, gid, AT_FDCWD, true };
    int rc = syscall(SC_chown, &params);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param fd 
 * @param uid 
 * @param gid 
 * @return int 
 */
int fchown(int fd, uid_t uid, gid_t gid)
{
    int rc = syscall(SC_fchown, fd, uid, gid);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param fd 
 * @param pathname 
 * @param uid 
 * @param gid 
 * @param flags 
 * @return int 
 */
int fchownat(int fd, char const* pathname, uid_t uid, gid_t gid, int flags)
{
    if (!pathname) {
        errno = EFAULT;
        return -1;
    }
    Syscall::SC_chown_params params { { pathname, strlen(pathname) }, uid, gid, fd, !(flags & AT_SYMLINK_NOFOLLOW) };
    int rc = syscall(SC_chown, &params);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

pid_t fork()
{
    __pthread_fork_prepare();

    int rc = syscall(SC_fork);
    if (rc == 0) {
        s_cached_tid = 0;
        s_cached_pid = 0;
        __pthread_fork_child();
    } else if (rc != -1) {
        __pthread_fork_parent();
    }
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

pid_t vfork()
{
    return fork();
}

/**
 * @param nochdir 
 * @param noclose 
 * @return int 
 */
int daemon(int nochdir, int noclose)
{
    pid_t pid = fork();
    if (pid < 0)
        return -1;

    if (pid > 0)
        _exit(0);

    pid = setsid();
    if (pid < 0)
        return -1;

    if (nochdir == 0)
        (void)chdir("/");

    if (noclose == 0) {
        int fd = open("/dev/null", O_WRONLY);
        if (fd < 0)
            return -1;
        (void)close(STDOUT_FILENO);
        (void)close(STDERR_FILENO);
        (void)dup2(fd, STDOUT_FILENO);
        (void)dup2(fd, STDERR_FILENO);
        (void)close(fd);
    }
    return 0;
}

/**
 * @param path 
 * @param argv 
 * @return int 
 */
int execv(char const* path, char* const argv[])
{
    return execve(path, argv, environ);
}

/**
 * @param filename 
 * @param argv 
 * @param envp 
 * @return int 
 */
int execve(char const* filename, char* const argv[], char* const envp[])
{
    size_t arg_count = 0;
    for (size_t i = 0; argv[i]; ++i)
        ++arg_count;

    size_t env_count = 0;
    for (size_t i = 0; envp[i]; ++i)
        ++env_count;

    auto copy_strings = [&](auto& vec, size_t count, auto& output) {
        output.length = count;
        for (size_t i = 0; vec[i]; ++i) {
            output.strings[i].characters = vec[i];
            output.strings[i].length = strlen(vec[i]);
        }
    };

    Syscall::SC_execve_params params;
    params.arguments.strings = (Syscall::StringArgument*)alloca(arg_count * sizeof(Syscall::StringArgument));
    params.environment.strings = (Syscall::StringArgument*)alloca(env_count * sizeof(Syscall::StringArgument));

    params.path = { filename, strlen(filename) };
    copy_strings(argv, arg_count, params.arguments);
    copy_strings(envp, env_count, params.environment);

    int rc = syscall(SC_execve, &params);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param filename 
 * @param argv 
 * @param envp 
 * @return int 
 */
int execvpe(char const* filename, char* const argv[], char* const envp[])
{
    if (strchr(filename, '/'))
        return execve(filename, argv, envp);

    ScopedValueRollback errno_rollback(errno);
    String path = getenv("PATH");
    if (path.is_empty())
        path = "/bin:/usr/bin";
    auto parts = path.split(':');
    for (auto& part : parts) {
        auto candidate = String::formatted("{}/{}", part, filename);
        int rc = execve(candidate.characters(), argv, envp);
        if (rc < 0 && errno != ENOENT) {
            errno_rollback.set_override_rollback_value(errno);
            dbgln("execvpe() failed on attempt ({}) with {}", candidate, strerror(errno));
            return rc;
        }
    }
    errno_rollback.set_override_rollback_value(ENOENT);
    dbgln("execvpe() leaving :(");
    return -1;
}

/**
 * @param filename 
 * @param argv 
 * @return int 
 */
int execvp(char const* filename, char* const argv[])
{
    int rc = execvpe(filename, argv, environ);
    int saved_errno = errno;
    dbgln("execvp({}, ...) about to return {} with errno={}", filename, rc, saved_errno);
    errno = saved_errno;
    return rc;
}

/**
 * @param filename 
 * @param arg0 
 * @param ... 
 * @return int 
 */
int execl(char const* filename, char const* arg0, ...)
{
    Vector<char const*, 16> args;
    args.append(arg0);

    va_list ap;
    va_start(ap, arg0);
    for (;;) {
        char const* arg = va_arg(ap, char const*);
        if (!arg)
            break;
        args.append(arg);
    }
    va_end(ap);
    args.append(nullptr);
    return execve(filename, const_cast<char* const*>(args.data()), environ);
}

/**
 * @param filename 
 * @param arg0 
 * @param ... 
 * @return int 
 */
int execle(char const* filename, char const* arg0, ...)
{
    Vector<char const*> args;
    args.append(arg0);

    va_list ap;
    va_start(ap, arg0);
    char const* arg;
    do {
        arg = va_arg(ap, char const*);
        args.append(arg);
    } while (arg);

    auto argv = const_cast<char* const*>(args.data());
    auto envp = const_cast<char* const*>(va_arg(ap, char* const*));
    va_end(ap);

    return execve(filename, argv, envp);
}

/**
 * @param filename 
 * @param arg0 
 * @param ... 
 * @return int 
 */
int execlp(char const* filename, char const* arg0, ...)
{
    Vector<char const*, 16> args;
    args.append(arg0);

    va_list ap;
    va_start(ap, arg0);
    for (;;) {
        char const* arg = va_arg(ap, char const*);
        if (!arg)
            break;
        args.append(arg);
    }
    va_end(ap);
    args.append(nullptr);
    return execvpe(filename, const_cast<char* const*>(args.data()), environ);
}

/**
 * @return uid_t 
 */
uid_t geteuid()
{
    return syscall(SC_geteuid);
}

/**
 * @return gid_t 
 */
gid_t getegid()
{
    return syscall(SC_getegid);
}

/**
 * @return uid_t 
 */
uid_t getuid()
{
    return syscall(SC_getuid);
}

/**
 * @return gid_t 
 */
gid_t getgid()
{
    return syscall(SC_getgid);
}

/**
 * @return pid_t 
 */
pid_t getpid()
{
    int cached_pid = s_cached_pid;
    if (!cached_pid) {
        cached_pid = syscall(SC_getpid);
        s_cached_pid = cached_pid;
    }
    return cached_pid;
}

/**
 * @return pid_t 
 */
pid_t getppid()
{
    return syscall(SC_getppid);
}

/**
 * @param ruid 
 * @param euid 
 * @param suid 
 * @return int 
 */
int getresuid(uid_t* ruid, uid_t* euid, uid_t* suid)
{
    return syscall(SC_getresuid, ruid, euid, suid);
}

/**
 * @param rgid 
 * @param egid 
 * @param sgid 
 * @return int 
 */
int getresgid(gid_t* rgid, gid_t* egid, gid_t* sgid)
{
    return syscall(SC_getresgid, rgid, egid, sgid);
}

/**
 * @param pid 
 * @return pid_t 
 */
pid_t getsid(pid_t pid)
{
    int rc = syscall(SC_getsid, pid);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @return pid_t 
 */
pid_t setsid()
{
    int rc = syscall(SC_setsid);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param fd 
 * @return pid_t 
 */
pid_t tcgetpgrp(int fd)
{
    pid_t pgrp;
    int rc = ioctl(fd, TIOCGPGRP, &pgrp);
    if (rc < 0)
        return rc;
    return pgrp;
}

/**
 * @param fd 
 * @param pgid 
 * @return int 
 */
int tcsetpgrp(int fd, pid_t pgid)
{
    return ioctl(fd, TIOCSPGRP, pgid);
}

/**
 * @param pid 
 * @param pgid 
 * @return int 
 */
int setpgid(pid_t pid, pid_t pgid)
{
    int rc = syscall(SC_setpgid, pid, pgid);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param pid 
 * @return pid_t 
 */
pid_t getpgid(pid_t pid)
{
    int rc = syscall(SC_getpgid, pid);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @return pid_t 
 */
pid_t getpgrp()
{
    int rc = syscall(SC_getpgrp);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param fd 
 * @param buf 
 * @param count 
 * @return ssize_t 
 */
ssize_t read(int fd, void* buf, size_t count)
{
    int rc = syscall(SC_read, fd, buf, count);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param fd 
 * @param buf 
 * @param count 
 * @param offset 
 * @return ssize_t 
 */
ssize_t pread(int fd, void* buf, size_t count, off_t offset)
{
    int rc = syscall(SC_pread, fd, buf, count, &offset);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param fd 
 * @param buf 
 * @param count 
 * @return ssize_t 
 */
ssize_t write(int fd, void const* buf, size_t count)
{
    int rc = syscall(SC_write, fd, buf, count);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param fd 
 * @param buf 
 * @param count 
 * @param offset 
 * @return ssize_t 
 */
ssize_t pwrite(int fd, void const* buf, size_t count, off_t offset)
{    
    off_t old_offset = lseek(fd, 0, SEEK_CUR);
    lseek(fd, offset, SEEK_SET);
    ssize_t nwritten = write(fd, buf, count);
    lseek(fd, old_offset, SEEK_SET);
    return nwritten;
}

/**
 * @param directory_name 
 * @param device_mode 
 * @param inode_number 
 * @param buffer 
 * @param size 
 * @return int 
 */
static int ttyname_r_for_directory(char const* directory_name, dev_t device_mode, ino_t inode_number, char* buffer, size_t size)
{
    DIR* dirstream = opendir(directory_name);
    if (!dirstream) {
        return -1;
    }

    auto close_dir_stream_on_exit = ScopeGuard([dirstream] {
        closedir(dirstream);
    });

    struct dirent* entry = nullptr;
    char* name_path = nullptr;

    while ((entry = readdir(dirstream)) != nullptr) {
        if (((ino_t)entry->d_ino == inode_number)
            && strcmp(entry->d_name, "stdin")
            && strcmp(entry->d_name, "stdout")
            && strcmp(entry->d_name, "stderr")) {

            size_t name_length = strlen(directory_name) + strlen(entry->d_name) + 1;

            if (name_length > size) {
                errno = ERANGE;
                return -1;
            }

            name_path = (char*)malloc(name_length);
            memset(name_path, 0, name_length);
            memcpy(name_path, directory_name, strlen(directory_name));
            memcpy(&name_path[strlen(directory_name)], entry->d_name, strlen(entry->d_name));
            struct stat st;
            if (lstat(name_path, &st) < 0) {
                free(name_path);
                name_path = nullptr;
                continue;
            }

            if (device_mode == st.st_rdev) {
                memset(buffer, 0, name_length);
                memcpy(buffer, name_path, name_length);
                free(name_path);
                return 0;
            }
        }
    }
    free(name_path);
    return -1;
}

/**
 * @param fd 
 * @param buffer 
 * @param size 
 * @return int 
 */
int ttyname_r(int fd, char* buffer, size_t size)
{
    struct stat stat;
    if (fstat(fd, &stat) < 0)
        return -1;
    dev_t major_minor_numbers = stat.st_rdev;
    ino_t inode_number = stat.st_ino;
    if (ttyname_r_for_directory("/dev/", major_minor_numbers, inode_number, buffer, size) < 0) {
        if (ttyname_r_for_directory("/dev/pts/", major_minor_numbers, inode_number, buffer, size) < 0) {
            errno = ENOTTY;
            return -1;
        }
    }
    return 0;
}

static char ttyname_buf[32];

/**
 * @param fd 
 * @return char* 
 */
char* ttyname(int fd)
{
    if (ttyname_r(fd, ttyname_buf, sizeof(ttyname_buf)) < 0)
        return nullptr;
    return ttyname_buf;
}

/**
 * @param fd 
 * @return int 
 */
int close(int fd)
{
    int rc = syscall(SC_close, fd);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param path 
 * @return int 
 */
int chdir(char const* path)
{
    if (!path) {
        errno = EFAULT;
        return -1;
    }
    int rc = syscall(SC_chdir, path, strlen(path));
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param fd 
 * @return int 
 */
int fchdir(int fd)
{
    int rc = syscall(SC_fchdir, fd);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param buffer 
 * @param size 
 * @return char* 
 */
char* getcwd(char* buffer, size_t size)
{
    if (buffer && size == 0) {
        errno = EINVAL;
        return nullptr;
    }

    bool self_allocated = false;
    if (!buffer) {
        size = size ? size : 64;
        buffer = (char*)malloc(size);
        self_allocated = true;
    }

    int rc = syscall(SC_getcwd, buffer, size);
    if (rc < 0) {
        if (self_allocated)
            free(buffer);
        errno = -rc;
        return nullptr;
    }

    size_t actual_size = static_cast<size_t>(rc);
    if (actual_size <= size) {
        return buffer;
    }

    if (!self_allocated) {
        errno = ERANGE;
        return nullptr;
    }

    free(buffer);
    size = actual_size;
    buffer = (char*)malloc(size);
    rc = syscall(SC_getcwd, buffer, size);
    if (rc < 0) {
        free(buffer);
        errno = -rc;
        return nullptr;
    }

    actual_size = static_cast<size_t>(rc);
    if (actual_size < size) {
        free(buffer);
        errno = EAGAIN;
        return nullptr;
    }

    return buffer;
}

/**
 * @param buf 
 * @return char* 
 */
char* getwd(char* buf)
{
    if (buf == nullptr) {
        errno = EINVAL;
        return nullptr;
    }
    auto* p = getcwd(buf, PATH_MAX);
    if (errno == ERANGE) {
        errno = ENAMETOOLONG;
    }
    return p;
}

/**
 * @param seconds 
 * @return unsigned int 
 */
unsigned int sleep(unsigned int seconds)
{
    struct timespec ts = { seconds, 0 };
    if (clock_nanosleep(CLOCK_MONOTONIC_COARSE, 0, &ts, nullptr) < 0)
        return ts.tv_sec;
    return 0;
}

/**
 * @param usec 
 * @return int 
 */
int usleep(useconds_t usec)
{
    struct timespec ts = { (long)(usec / 1000000), (long)(usec % 1000000) * 1000 };
    return clock_nanosleep(CLOCK_MONOTONIC_COARSE, 0, &ts, nullptr);
}

/**
 * @param buffer 
 * @param size 
 * @return int 
 */
int gethostname(char* buffer, size_t size)
{
    int rc = syscall(SC_gethostname, buffer, size);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param hostname 
 * @param size 
 * @return int 
 */
int sethostname(char const* hostname, ssize_t size)
{
    int rc = syscall(SC_sethostname, hostname, size);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param path 
 * @param buffer 
 * @param size 
 * @return ssize_t 
 */
ssize_t readlink(char const* path, char* buffer, size_t size)
{
    Syscall::SC_readlink_params params { { path, strlen(path) }, { buffer, size } };
    int rc = syscall(SC_readlink, &params);

    __RETURN_WITH_ERRNO(rc, min((size_t)rc, size), -1);
}

/**
 * @param fd 
 * @param offset 
 * @param whence 
 * @return off_t 
 */
off_t lseek(int fd, off_t offset, int whence)
{
    int rc = syscall(SC_lseek, fd, &offset, whence);
    __RETURN_WITH_ERRNO(rc, offset, -1);
}

/**
 * @param old_path 
 * @param new_path 
 * @return int 
 */
int link(char const* old_path, char const* new_path)
{
    if (!old_path || !new_path) {
        errno = EFAULT;
        return -1;
    }
    Syscall::SC_link_params params { { old_path, strlen(old_path) }, { new_path, strlen(new_path) } };
    int rc = syscall(SC_link, &params);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param pathname 
 * @return int 
 */
int unlink(char const* pathname)
{
    return unlinkat(AT_FDCWD, pathname, 0);
}

/**
 * @param dirfd 
 * @param pathname 
 * @param flags 
 * @return int 
 */
int unlinkat(int dirfd, char const* pathname, int flags)
{
    int rc = syscall(SC_unlink, dirfd, pathname, strlen(pathname), flags);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param target 
 * @param linkpath 
 * @return int 
 */
int symlink(char const* target, char const* linkpath)
{
    if (!target || !linkpath) {
        errno = EFAULT;
        return -1;
    }
    Syscall::SC_symlink_params params { { target, strlen(target) }, { linkpath, strlen(linkpath) } };
    int rc = syscall(SC_symlink, &params);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param pathname 
 * @return int 
 */
int rmdir(char const* pathname)
{
    if (!pathname) {
        errno = EFAULT;
        return -1;
    }
    int rc = syscall(SC_rmdir, pathname, strlen(pathname));
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param fd 
 * @return int 
 */
int isatty(int fd)
{
    return fcntl(fd, F_ISTTY);
}

/**
 * @param old_fd 
 * @return int 
 */
int dup(int old_fd)
{
    return fcntl(old_fd, F_DUPFD, 0);
}

/**
 * @param old_fd 
 * @param new_fd 
 * @return int 
 */
int dup2(int old_fd, int new_fd)
{
    int rc = syscall(SC_dup2, old_fd, new_fd);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param size 
 * @param list 
 * @return int 
 */
int setgroups(size_t size, gid_t const* list)
{
    int rc = syscall(SC_setgroups, size, list);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param size 
 * @param list 
 * @return int 
 */
int getgroups(int size, gid_t list[])
{
    int rc = syscall(SC_getgroups, size, list);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param pipefd 
 * @return int 
 */
int pipe(int pipefd[2])
{
    return pipe2(pipefd, 0);
}

/**
 * @param pipefd 
 * @param flags 
 * @return int 
 */
int pipe2(int pipefd[2], int flags)
{
    int rc = syscall(SC_pipe, pipefd, flags);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param seconds 
 * @return unsigned int 
 */
unsigned int alarm(unsigned int seconds)
{
    return syscall(SC_alarm, seconds);
}

/**
 * @param euid 
 * @return int 
 */
int seteuid(uid_t euid)
{
    int rc = syscall(SC_seteuid, euid);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param egid 
 * @return int 
 */
int setegid(gid_t egid)
{
    int rc = syscall(SC_setegid, egid);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param uid 
 * @return int 
 */
int setuid(uid_t uid)
{
    int rc = syscall(SC_setuid, uid);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param gid 
 * @return int 
 */
int setgid(gid_t gid)
{
    int rc = syscall(SC_setgid, gid);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param ruid 
 * @param euid 
 * @return int 
 */
int setreuid(uid_t ruid, uid_t euid)
{
    int rc = syscall(SC_setreuid, ruid, euid);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param ruid 
 * @param euid 
 * @param suid 
 * @return int 
 */
int setresuid(uid_t ruid, uid_t euid, uid_t suid)
{
    int rc = syscall(SC_setresuid, ruid, euid, suid);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param rgid 
 * @param egid 
 * @param sgid 
 * @return int 
 */
int setresgid(gid_t rgid, gid_t egid, gid_t sgid)
{
    int rc = syscall(SC_setresgid, rgid, egid, sgid);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param pathname 
 * @param mode 
 * @return int 
 */
int access(char const* pathname, int mode)
{
    if (!pathname) {
        errno = EFAULT;
        return -1;
    }
    int rc = syscall(SC_access, pathname, strlen(pathname), mode);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param pathname 
 * @param mode 
 * @param dev 
 * @return int 
 */
int mknod(char const* pathname, mode_t mode, dev_t dev)
{
    if (!pathname) {
        errno = EFAULT;
        return -1;
    }
    Syscall::SC_mknod_params params { { pathname, strlen(pathname) }, mode, dev };
    int rc = syscall(SC_mknod, &params);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param fd 
 * @param name 
 * @return long 
 */
long fpathconf([[maybe_unused]] int fd, int name)
{
    switch (name) {
    case _PC_NAME_MAX:
        return NAME_MAX;
    case _PC_PATH_MAX:
        return PATH_MAX;
    case _PC_VDISABLE:
        return _POSIX_VDISABLE;
    case _PC_LINK_MAX:
        return LINK_MAX;
    }

    VERIFY_NOT_REACHED();
}

/**
 * @param path 
 * @param name 
 * @return long 
 */
long pathconf([[maybe_unused]] char const* path, int name)
{
    switch (name) {
    case _PC_NAME_MAX:
        return NAME_MAX;
    case _PC_PATH_MAX:
        return PATH_MAX;
    case _PC_PIPE_BUF:
        return PIPE_BUF;
    case _PC_LINK_MAX:
        return LINK_MAX;
    }

    VERIFY_NOT_REACHED();
}

/**
 * @param status 
 */
void _exit(int status)
{
    syscall(SC_exit, status);
    VERIFY_NOT_REACHED();
}

void sync()
{
    syscall(SC_sync);
}

static String getlogin_buffer;

char* getlogin()
{
    if (getlogin_buffer.is_null()) {
        if (auto* passwd = getpwuid(getuid())) {
            getlogin_buffer = String(passwd->pw_name);
        }
        endpwent();
    }
    return const_cast<char*>(getlogin_buffer.characters());
}

/**
 * @param fd 
 * @param length 
 * @return int 
 */
int ftruncate(int fd, off_t length)
{
    int rc = syscall(SC_ftruncate, fd, &length);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param path 
 * @param length 
 * @return int 
 */
int truncate(char const* path, off_t length)
{
    int fd = open(path, O_RDWR | O_CREAT, 0666);
    if (fd < 0)
        return fd;
    int rc = ftruncate(fd, length);
    int saved_errno = errno;
    if (int close_rc = close(fd); close_rc < 0)
        return close_rc;
    errno = saved_errno;
    return rc;
}

int gettid()
{
    int cached_tid = s_cached_tid;
    if (!cached_tid) {
        cached_tid = syscall(SC_gettid);
        s_cached_tid = cached_tid;
    }
    return cached_tid;
}

int sysbeep()
{
    int rc = syscall(SC_beep);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param fd 
 * @return int 
 */
int fsync(int fd)
{
    int rc = syscall(SC_fsync, fd);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param source_fd 
 * @param target 
 * @param fs_type 
 * @param flags 
 * @return int 
 */
int mount(int source_fd, char const* target, char const* fs_type, int flags)
{
    if (!target || !fs_type) {
        errno = EFAULT;
        return -1;
    }

    Syscall::SC_mount_params params {
        { target, strlen(target) },
        { fs_type, strlen(fs_type) },
        source_fd,
        flags
    };
    int rc = syscall(SC_mount, &params);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param mountpoint 
 * @return int 
 */
int umount(char const* mountpoint)
{
    int rc = syscall(SC_umount, mountpoint, strlen(mountpoint));
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

void dump_backtrace()
{
    syscall(SC_dump_backtrace);
}

/**
 * @brief Get the process name object
 * 
 * @param buffer 
 * @param buffer_size 
 * @return int 
 */
int get_process_name(char* buffer, int buffer_size)
{
    int rc = syscall(SC_get_process_name, buffer, buffer_size);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @brief Set the process name object
 * 
 * @param name 
 * @param name_length 
 * @return int 
 */
int set_process_name(char const* name, size_t name_length)
{
    int rc = syscall(SC_set_process_name, name, name_length);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param promises 
 * @param execpromises 
 * @return int 
 */
int pledge(char const* promises, char const* execpromises)
{
    Syscall::SC_pledge_params params {
        { promises, promises ? strlen(promises) : 0 },
        { execpromises, execpromises ? strlen(execpromises) : 0 }
    };
    int rc = syscall(SC_pledge, &params);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param path 
 * @param permissions 
 * @return int 
 */
int unveil(char const* path, char const* permissions)
{
    Syscall::SC_unveil_params params {
        { path, path ? strlen(path) : 0 },
        { permissions, permissions ? strlen(permissions) : 0 }
    };
    int rc = syscall(SC_unveil, &params);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

/**
 * @param prompt 
 * @return char* 
 */
char* getpass(char const* prompt)
{
    dbgln("FIXME: getpass('{}')", prompt);
    TODO();
}

/**
 * @param name 
 * @return long 
 */
long sysconf(int name)
{
    int rc = syscall(SC_sysconf, name);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

int getpagesize()
{
    return PAGE_SIZE;
}

int pause()
{
    return select(0, nullptr, nullptr, nullptr, nullptr);
}

/**
 * @param path 
 * @return int 
 */
int chroot(char const* path)
{
    dbgln("FIXME: chroot(\"{}\")", path);
    return -1;
}

int getdtablesize()
{
    rlimit dtablesize;
    int rc = getrlimit(RLIMIT_NOFILE, &dtablesize);
    __RETURN_WITH_ERRNO(rc, dtablesize.rlim_cur, rc);
}
}
