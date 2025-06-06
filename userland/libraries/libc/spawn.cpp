/**
 * @file spawn.cpp
 * @author Krisna Pranav
 * @brief spawn
 * @version 6.0
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <spawn.h>

#include <mods/function.h>
#include <mods/vector.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

struct posix_spawn_file_actions_state {
    Vector<Function<int()>, 4> actions;
}; // struct posix_spawn_file_actions_state

extern "C" {

/**
 * @param path 
 * @param file_actions 
 * @param attr 
 * @param argv 
 * @param envp 
 * @param exec 
 */
[[noreturn]] static void posix_spawn_child(char const* path, posix_spawn_file_actions_t const* file_actions, posix_spawnattr_t const* attr, char* const argv[], char* const envp[], int (*exec)(char const*, char* const[], char* const[]))
{
    if (attr) {
        short flags = attr->flags;
        if (flags & POSIX_SPAWN_RESETIDS) {
            if (seteuid(getuid()) < 0) {
                perror("posix_spawn seteuid");
                _exit(127);
            }
            if (setegid(getgid()) < 0) {
                perror("posix_spawn setegid");
                _exit(127);
            }
        }
        if (flags & POSIX_SPAWN_SETPGROUP) {
            if (setpgid(0, attr->pgroup) < 0) {
                perror("posix_spawn setpgid");
                _exit(127);
            }
        }
        if (flags & POSIX_SPAWN_SETSCHEDPARAM) {
            if (sched_setparam(0, &attr->schedparam) < 0) {
                perror("posix_spawn sched_setparam");
                _exit(127);
            }
        }
        if (flags & POSIX_SPAWN_SETSIGDEF) {
            struct sigaction default_action;
            default_action.sa_flags = 0;
            sigemptyset(&default_action.sa_mask);
            default_action.sa_handler = SIG_DFL;

            sigset_t sigdefault = attr->sigdefault;
            for (int i = 0; i < NSIG; ++i) {
                if (sigismember(&sigdefault, i) && sigaction(i, &default_action, nullptr) < 0) {
                    perror("posix_spawn sigaction");
                    _exit(127);
                }
            }
        }
        if (flags & POSIX_SPAWN_SETSIGMASK) {
            if (sigprocmask(SIG_SETMASK, &attr->sigmask, nullptr) < 0) {
                perror("posix_spawn sigprocmask");
                _exit(127);
            }
        }
        if (flags & POSIX_SPAWN_SETSID) {
            if (setsid() < 0) {
                perror("posix_spawn setsid");
                _exit(127);
            }
        }

    }

    if (file_actions) {
        for (auto const& action : file_actions->state->actions) {
            if (action() < 0) {
                perror("posix_spawn file action");
                _exit(127);
            }
        }
    }

    exec(path, argv, envp);
    perror("posix_spawn exec");
    _exit(127);
}

/**
 * @param out_pid 
 * @param path 
 * @param file_actions 
 * @param attr 
 * @param argv 
 * @param envp 
 * @return int 
 */
int posix_spawn(pid_t* out_pid, char const* path, posix_spawn_file_actions_t const* file_actions, posix_spawnattr_t const* attr, char* const argv[], char* const envp[])
{
    pid_t child_pid = fork();
    if (child_pid < 0)
        return errno;

    if (child_pid != 0) {
        *out_pid = child_pid;
        return 0;
    }

    posix_spawn_child(path, file_actions, attr, argv, envp, execve);
}

/**
 * @param out_pid 
 * @param path 
 * @param file_actions 
 * @param attr 
 * @param argv 
 * @param envp 
 * @return int 
 */
int posix_spawnp(pid_t* out_pid, char const* path, posix_spawn_file_actions_t const* file_actions, posix_spawnattr_t const* attr, char* const argv[], char* const envp[])
{
    pid_t child_pid = fork();
    if (child_pid < 0)
        return errno;

    if (child_pid != 0) {
        *out_pid = child_pid;
        return 0;
    }

    posix_spawn_child(path, file_actions, attr, argv, envp, execvpe);
}

/**
 * @param actions 
 * @param path 
 * @return int 
 */
int posix_spawn_file_actions_addchdir(posix_spawn_file_actions_t* actions, char const* path)
{
    actions->state->actions.append([path]() { return chdir(path); });
    return 0;
}

/**
 * @param actions 
 * @param fd 
 * @return int 
 */
int posix_spawn_file_actions_addfchdir(posix_spawn_file_actions_t* actions, int fd)
{
    actions->state->actions.append([fd]() { return fchdir(fd); });
    return 0;
}

/**
 * @param actions 
 * @param fd 
 * @return int 
 */
int posix_spawn_file_actions_addclose(posix_spawn_file_actions_t* actions, int fd)
{
    actions->state->actions.append([fd]() { return close(fd); });
    return 0;
}

/**
 * @param actions 
 * @param old_fd 
 * @param new_fd 
 * @return int 
 */
int posix_spawn_file_actions_adddup2(posix_spawn_file_actions_t* actions, int old_fd, int new_fd)
{
    actions->state->actions.append([old_fd, new_fd]() { return dup2(old_fd, new_fd); });
    return 0;
}

/**
 * @param actions 
 * @param want_fd 
 * @param path 
 * @param flags 
 * @param mode 
 * @return int 
 */
int posix_spawn_file_actions_addopen(posix_spawn_file_actions_t* actions, int want_fd, char const* path, int flags, mode_t mode)
{
    actions->state->actions.append([want_fd, path, flags, mode]() {
        int opened_fd = open(path, flags, mode);
        if (opened_fd < 0 || opened_fd == want_fd)
            return opened_fd;
        if (int rc = dup2(opened_fd, want_fd); rc < 0)
            return rc;
        return close(opened_fd);
    });
    return 0;
}

/**
 * @param actions 
 * @return int 
 */
int posix_spawn_file_actions_destroy(posix_spawn_file_actions_t* actions)
{
    delete actions->state;
    return 0;
}

/**
 * @param actions 
 * @return int 
 */
int posix_spawn_file_actions_init(posix_spawn_file_actions_t* actions)
{
    actions->state = new posix_spawn_file_actions_state;
    return 0;
}

/**
 * @return int 
 */
int posix_spawnattr_destroy(posix_spawnattr_t*)
{
    return 0;
}

/**
 * @param attr 
 * @param out_flags 
 * @return int 
 */
int posix_spawnattr_getflags(posix_spawnattr_t const* attr, short* out_flags)
{
    *out_flags = attr->flags;
    return 0;
}

/**
 * @param attr 
 * @param out_pgroup 
 * @return int 
 */
int posix_spawnattr_getpgroup(posix_spawnattr_t const* attr, pid_t* out_pgroup)
{
    *out_pgroup = attr->pgroup;
    return 0;
}

/**
 * @param attr 
 * @param out_schedparam 
 * @return int 
 */
int posix_spawnattr_getschedparam(posix_spawnattr_t const* attr, struct sched_param* out_schedparam)
{
    *out_schedparam = attr->schedparam;
    return 0;
}

/**
 * @param attr 
 * @param out_schedpolicy 
 * @return int 
 */
int posix_spawnattr_getschedpolicy(posix_spawnattr_t const* attr, int* out_schedpolicy)
{
    *out_schedpolicy = attr->schedpolicy;
    return 0;
}

/**
 * @param attr 
 * @param out_sigdefault 
 * @return int 
 */
int posix_spawnattr_getsigdefault(posix_spawnattr_t const* attr, sigset_t* out_sigdefault)
{
    *out_sigdefault = attr->sigdefault;
    return 0;
}

/**
 * @param attr 
 * @param out_sigmask 
 * @return int 
 */
int posix_spawnattr_getsigmask(posix_spawnattr_t const* attr, sigset_t* out_sigmask)
{
    *out_sigmask = attr->sigmask;
    return 0;
}

/**
 * @param attr 
 * @return int 
 */
int posix_spawnattr_init(posix_spawnattr_t* attr)
{
    attr->flags = 0;
    attr->pgroup = 0;

    sigemptyset(&attr->sigdefault);
    
    return 0;
}

/**
 * @param attr 
 * @param flags 
 * @return int 
 */
int posix_spawnattr_setflags(posix_spawnattr_t* attr, short flags)
{
    if (flags & ~(POSIX_SPAWN_RESETIDS | POSIX_SPAWN_SETPGROUP | POSIX_SPAWN_SETSCHEDPARAM | POSIX_SPAWN_SETSCHEDULER | POSIX_SPAWN_SETSIGDEF | POSIX_SPAWN_SETSIGMASK | POSIX_SPAWN_SETSID))
        return EINVAL;

    attr->flags = flags;
    return 0;
}

/**
 * @param attr 
 * @param pgroup 
 * @return int 
 */
int posix_spawnattr_setpgroup(posix_spawnattr_t* attr, pid_t pgroup)
{
    attr->pgroup = pgroup;
    return 0;
}

/**
 * @param attr 
 * @param schedparam 
 * @return int 
 */
int posix_spawnattr_setschedparam(posix_spawnattr_t* attr, const struct sched_param* schedparam)
{
    attr->schedparam = *schedparam;
    return 0;
}

/**
 * @param attr 
 * @param schedpolicy 
 * @return int 
 */
int posix_spawnattr_setschedpolicy(posix_spawnattr_t* attr, int schedpolicy)
{
    attr->schedpolicy = schedpolicy;
    return 0;
}

/**
 * @param attr 
 * @param sigdefault 
 * @return int 
 */
int posix_spawnattr_setsigdefault(posix_spawnattr_t* attr, sigset_t const* sigdefault)
{
    attr->sigdefault = *sigdefault;
    return 0;
}

/**
 * @param attr 
 * @param sigmask 
 * @return int 
 */
int posix_spawnattr_setsigmask(posix_spawnattr_t* attr, sigset_t const* sigmask)
{
    attr->sigmask = *sigmask;
    return 0;
}
}
