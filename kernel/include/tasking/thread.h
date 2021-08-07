/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef _KERNEL_TASKING_THREAD_H
#define _KERNEL_TASKING_THREAD_H

// includes
#include <drivers/generic/fpu.h>
#include <fs/vfs.h>
#include <libkernel/lock.h>
#include <libkernel/types.h>
#include <platform/generic/tasking/context.h>
#include <platform/generic/tasking/trapframe.h>
#include <tasking/signal.h>
#include <time/time_manager.h>

enum THREAD_STATUS {
    THREAD_INVALID = 0,
    THREAD_RUNNING,
    THREAD_DEAD,
    THREAD_STOPPED,
    THREAD_BLOCKED,
    THREAD_DYING,
};

struct thread;

struct blocker {
    int reason;
    int (*should_unblock)(struct thread* p);
    bool should_unblock_for_signal;
};

typedef struct blocker blocker_t;

#define THREAD_PER_NODE (128)

struct thread_list_node {
    thread_t thread_storage[THREADS_PER_NODE];
    struct thread_list_node* next;
    int empty_spots;
};

typedef struct thread_list_node thread_list_node_t;

struct thread_list {
    struct thread_list_node* head;
};


int init_join_blocker(thread_t* p);


#endif 