/**
 * @file spawn.h
 * @author Krisna Pranav
 * @brief spawn
 * @version 1.0
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
    POSIX_SPAN_SETPGROUP = 1 << 1,
    POSIX_SPAWN_SETSCHEDPARAM = 1 << 2,
    POSIX_SPAWN_SETSCHEDULE = 1 << 3,
    POSIX_SPAWN_SETSID = 1 << 4,
}; // enum

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

__END_DECLS