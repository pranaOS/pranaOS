/**
 * @file ptrace.h
 * @author Krisna Pranav
 * @brief PTrace
 * @version 6.0
 * @date 2023-06-28
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <sys/types.h>

__BEGIN_DECLS

#define PT_TRACE_ME 1
#define PT_ATTACH 2
#define PT_CONTINUE 3
#define PT_SYSCALL 4
#define PT_GETREGS 5
#define PT_DETACH 6
#define PT_PEEK 7
#define PT_POKE 8
#define PT_SETREGS 9

/**
 * @brief ptrace
 * 
 * @param request 
 * @param tid 
 * @param addr 
 * @param data 
 * @return int 
 */
int ptrace(int request, pid_t tid, void* addr, int data);

__END_DECLS
