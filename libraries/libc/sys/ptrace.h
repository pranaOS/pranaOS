/**
 * @file ptrace.h
 * @author Krisna Pranav
 * @brief PTrace
 * @version 1.0
 * @date 2023-06-28
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <sys/types.h>

#define PT_TRACE_ME 1 
#define PT_ATTACH 2
#define PT_CONTINUE 3
#define PT_SYSCALL 4 
#define PT_GETREGS 5
#define PT_DETACH 6

/**
 * @brief ptrace
 * 
 * @param request 
 * @param pid 
 * @param addr 
 * @param data 
 * @return int 
 */
int ptrace(int request, pid_t pid, void* addr, int data);
