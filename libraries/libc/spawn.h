/**
 * @file spawn.h
 * @author Krisna Pranav
 * @brief spawn
 * @version 6.0
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <sched.h>
#include <signal.h>
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

enum 
{
    POSIX_SPAWN_RESETIDS = 1 << 0,
    POSIX_SPAWN_SETPGROUP = 1 << 1,

    POSIX_SPAWN_SETSCHEDPARAM = 1 << 2,
    POSIX_SPAWN_SETSCHEDULER = 1 << 3,

    POSIX_SPAWN_SETSIGDEF = 1 << 4,
    POSIX_SPAWN_SETSIGMASK = 1 << 5,

    POSIX_SPAWN_SETSID = 1 << 6,
}; // enum

/// @brief: POSIX_SPAN_SETSID
#define POSIX_SPAWN_SETSID POSIX_SPAWN_SETSID

struct posix_spawn_file_actions_state;

typedef struct 
{
    struct posix_spawn_file_actions_state* state;
} posix_spawn_file_actions_t;

typedef struct 
{
    short flags;
    pid_t pgroup;
    struct sched_param schedparam;
    int schedpolicy;
    sigset_t sigdefault;
    sigset_t sigmask;
} posix_spawnattr_t;


/**
 * @param argv 
 * @param envp 
 * @return int 
 */
int posix_spawn(pid_t*, const char*, const posix_spawn_file_actions_t*, const posix_spawnattr_t*, char* const argv[], char* const envp[]);

/**
 * @param argv 
 * @param envp 
 * @return int 
 */
int posix_spawnp(pid_t*, const char*, const posix_spawn_file_actions_t*, const posix_spawnattr_t*, char* const argv[], char* const envp[]);

/**
 * @return int 
 */
int posix_spawn_file_actions_addchdir(posix_spawn_file_actions_t*, const char*);

/**
 * @return int 
 */
int posix_spawn_file_actions_addfchdir(posix_spawn_file_actions_t*, int);

/**
 * @return int 
 */
int posix_spawn_file_actions_addclose(posix_spawn_file_actions_t*, int);

/**
 * @param old_fd 
 * @param new_fd 
 * @return int 
 */
int posix_spawn_file_actions_adddup2(posix_spawn_file_actions_t*, int old_fd, int new_fd);

/**
 * @param fd 
 * @param flags 
 * @return int 
 */
int posix_spawn_file_actions_addopen(posix_spawn_file_actions_t*, int fd, const char*, int flags, mode_t);

/**
 * @return int 
 */
int posix_spawn_file_actions_destroy(posix_spawn_file_actions_t*);

/**
 * @return int 
 */
int posix_spawn_file_actions_init(posix_spawn_file_actions_t*);

/**
 * @return int 
 */
int posix_spawnattr_destroy(posix_spawnattr_t*);

/**
 * @return int 
 */
int posix_spawnattr_getflags(const posix_spawnattr_t*, short*);

/**
 * @return int 
 */
int posix_spawnattr_getpgroup(const posix_spawnattr_t*, pid_t*);

/**
 * @return int 
 */
int posix_spawnattr_getschedparam(const posix_spawnattr_t*, struct sched_param*);

/**
 * @return int 
 */
int posix_spawnattr_getschedpolicy(const posix_spawnattr_t*, int*);

/**
 * @return int 
 */
int posix_spawnattr_getsigdefault(const posix_spawnattr_t*, sigset_t*);

/**
 * @return int 
 */
int posix_spawnattr_getsigmask(const posix_spawnattr_t*, sigset_t*);

/**
 * @return int 
 */
int posix_spawnattr_init(posix_spawnattr_t*);

/**
 * @return int 
 */
int posix_spawnattr_setflags(posix_spawnattr_t*, short);

/**
 * @return int 
 */
int posix_spawnattr_setpgroup(posix_spawnattr_t*, pid_t);

/**
 * @return int 
 */
int posix_spawnattr_setschedparam(posix_spawnattr_t*, const struct sched_param*);

/**
 * @return int 
 */
int posix_spawnattr_setschedpolicy(posix_spawnattr_t*, int);

/**
 * @return int 
 */
int posix_spawnattr_setsigdefault(posix_spawnattr_t*, const sigset_t*);

/**
 * @return int 
 */
int posix_spawnattr_setsigmask(posix_spawnattr_t*, const sigset_t*);

__END_DECLS
