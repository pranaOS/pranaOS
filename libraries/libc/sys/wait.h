/**
 * @file wait.h
 * @author Krisna Pranav
 * @brief Wait
 * @version 1.0
 * @date 2023-07-31
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <sys/cdefs.h>
#include <sys/types.h>
#include <signal.h>

__BEGIN_DECLS

#define WNOHANG 1
#define WUNTRACED 2 
#define WSTOPPED WUNTRACED
#define WEXITED 4
#define WCONTINUED 8

typedef enum 
{
    P_ALL = 1,
    P_PID,
    P_GID
} idtype_t;

/**
 * @param wstatus 
 * @param options 
 * @return pid_t 
 */
pid_t waitpid(pid_t, int* wstatus, int options);

/**
 * @param wstatus 
 * @return pid_t 
 */
pid_t wait(int* wstatus);

/**
 * @param idtype 
 * @param id 
 * @param infop 
 * @param options 
 * @return int 
 */
int waitid(idtype_t idtype, id_t id, siginfo_t* infop, int options);

__END_DECLS