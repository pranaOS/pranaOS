/**
 * @file ptrace.h
 * @author Krisna Pranav
 * @brief PTrace
 * @version 6.0
 * @date 2023-06-28
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/api/posix/sys/ptrace.h>

__BEGIN_DECLS

/**
 * @param request 
 * @param tid 
 * @param addr 
 * @param data 
 * @return long 
 */
long ptrace(int request, pid_t tid, void* addr, void* data);

__END_DECLS
