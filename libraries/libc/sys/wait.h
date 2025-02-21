/**
 * @file wait.h
 * @author Krisna Pranav
 * @brief Wait
 * @version 6.0
 * @date 2023-07-31
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/api/posix/signal.h>
#include <kernel/api/posix/sys/wait.h>

__BEGIN_DECLS

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
