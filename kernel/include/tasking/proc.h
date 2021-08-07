/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef _KERNEL_TASKING_PROC_H
#define _KERNEL_TASKING_PROC_H

// includes
#include <algo/dynamic_array.h>
#include <fs/vfs.h>
#include <io/tty/tty.h>
#include <libkernel/atomic.h>
#include <libkernel/lock.h>
#include <libkernel/types.h>
#include <mem/vmm/vmm.h>
#include <mem/vmm/zoner.h>

#define MAX_PROCESS_COUNT 1024
#define MAX_OPENED_FILES 16

struct blocker;

enum ZONE_FLAGS {
    ZONE_WRITABLE = 0x1,
    ZONE_READABLE = 0x2,
    ZONE_EXECUTABLE = 0x4,
    ZONE_NOT_CACHEABLE = 0x8,
    ZONE_COW = 0x10,
    ZONE_USER = 0x20,
};

enum ZONE_TYPES {
    ZONE_TYPE_NULL = 0x0,
    ZONE_TYPE_CODE = 0x1,
    ZONE_TYPE_DATA = 0x2,
    ZONE_TYPE_STACK = 0x4,
    ZONE_TYPE_BSS = 0x8,
    ZONE_TYPE_DEVICE = 0x10,
    ZONE_TYPE_MAPPED = 0x20,
    ZONE_TYPE_MAPPED_FILE_PRIVATLY = 0x40,
    ZONE_TYPE_MAPPED_FILE_SHAREDLY = 0x80,
};

struct proc_zone {
    uint32_t start;
    uint32_t len;
    uint32_t type;
    uint32_t flags;
    dentry_t* file;
    uint32_t offset;
};
typedef struct proc_zone proc_zone_t;

enum PROC_STATUS {
    PROC_INVALID = 0,
    PROC_ALIVE,
    PROC_DEAD,
    PROC_DYING,
};

struct thread;
struct proc {
    pdirectory_t* pdir;
    pid_t pid;
    pid_t ppid;
    pid_t pgid;
    uint32_t prio;
    uint32_t status;
    struct thread* main_thread;
    lock_t lock;

    uid_t uid;
    gid_t gid;
    uid_t euid;
    gid_t egid;
    uid_t suid;
    gid_t sgid;

    dynamic_array_t zones;

    dentry_t* proc_file;
    dentry_t* cwd;
    file_descriptor_t* fds;
    tty_entry_t* tty;

    bool is_kthread;
};

typedef struct proc proc_t;



#endif 