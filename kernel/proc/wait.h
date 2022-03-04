/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <stdint.h>
#include <utilities/list.h>
#include <libc/sys/types.h>

#define WNOHANG 0x00000001
#define WUNTRACED 0x00000002
#define WSTOPPED WUNTRACED
#define WEXITED 0x00000004
#define WCONTINUED 0x00000008
#define WNOWAIT 0x01000000 

#define CLD_EXITED 1	
#define CLD_KILLED 2	
#define CLD_DUMPED 3	
#define CLD_TRAPPED 4	
#define CLD_STOPPED 5	
#define CLD_CONTINUED 6 

#define P_ALL 0
#define P_PID 1
#define P_PGID 2

#define WSEXITED 0x0001
#define WSSIGNALED 0x0002
#define WSCOREDUMP 0x0004
#define WSSTOPPED 0x0008
#define WSCONTINUED 0x0010

/**
 * @brief infop
 * 
 */
struct infop {
    pid_t si_pid;
    int32_t si_signo;
    int32_t si_status;
    int32_t si_code;
};

struct thread;

/**
 * @brief wait queue
 * 
 */
typedef void (*wait_queue_func)(struct thread *);

/**
 * @brief queue head
 * 
 */
struct wait_queue_head {
    struct list_head list;
};