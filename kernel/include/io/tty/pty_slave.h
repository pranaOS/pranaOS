/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <algo/sync_ringbuffer.h>

#ifndef PTYS_COUNT
#define PTYS_COUNT 4
#endif

struct pty_master_entry;
struct pty_slave_entry {
    int inode_indx;
    struct pty_master_entry* ptm;
    sync_ringbuffer_t buffer;
};
typedef struct pty_slave_entry pty_slave_entry_t;

extern pty_slave_entry_t pty_slaves[PTYS_COUNT];

int pty_slave_create(int id, struct pty_master_entry* ptm);
