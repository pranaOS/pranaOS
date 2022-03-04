/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <ak/list.h>
#include <libc/sys/types.h>
#include <stdint.h>

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
 * @brief wait queue func
 * 
 */
typedef void (*wait_queue_func)(struct thread *);

/**
 * @brief wait queue head
 * 
 */
struct wait_queue_head {
	struct list_head list;
};

/**
 * @brief wait queue entry
 * 
 */
struct wait_queue_entry {
	struct thread *thread;
	wait_queue_func func;
	struct list_head sibling;
};

/**
 * @brief current thread
 * 
 */
extern volatile struct thread *current_thread;

/**
 * @brief schedule
 * 
 */
extern void schedule();

/**
 * @brief define wait
 * 
 */
#define DEFINE_WAIT(name)            \
	struct wait_queue_entry name = { \
		.thread = current_thread,    \
		.func = poll_wakeup,         \
	}

/**
 * @brief wait until
 * 
 */
#define wait_until(cond) ({                            \
	for (; !(cond);)                                   \
	{                                                  \
		update_thread(current_thread, THREAD_WAITING); \
		schedule();                                    \
	}                                                  \
})

/**
 * @brief wait until with prework
 * 
 */
#define wait_until_with_prework(cond, prework) ({      \
	for (; !(cond);)                                   \
	{                                                  \
		prework;                                       \
		update_thread(current_thread, THREAD_WAITING); \
		schedule();                                    \
	}                                                  \
})

/**
 * @brief wait until with setup
 * 
 */
#define wait_until_with_setup(cond, prework, afterwork) ({ \
	for (; !(cond);)                                       \
	{                                                      \
		prework;                                           \
		update_thread(current_thread, THREAD_WAITING);     \
		schedule();                                        \
		afterwork;                                         \
	}                                                      \
})

/**
 * @brief wait event
 * 
 */
#define wait_event(wh, cond) ({                  \
	DEFINE_WAIT(__wait);                         \
	list_add_tail(&__wait.sibling, &(wh)->list); \
	wait_until(cond);                            \
	list_del(&__wait.sibling);                   \
})