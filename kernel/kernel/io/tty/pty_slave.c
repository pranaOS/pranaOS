/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <fs/devfs/devfs.h>
#include <io/tty/pty_master.h>
#include <io/tty/pty_slave.h>
#include <libkernel/libkernel.h>
#include <libkernel/log.h>

pty_slave_entry_t pty_slaves[PTYS_COUNT];

static pty_slave_entry_t* _pts_get(dentry_t* dentry)
{
    for (int i = 0; i < PTYS_COUNT; i++) {
        if (dentry->inode_indx == pty_slaves[i].inode_indx) {
            return &pty_slaves[i];
        }
    }
    return NULL;
}

bool pty_slave_can_read(dentry_t* dentry, uint32_t start)
{
    pty_slave_entry_t* pts = _pts_get(dentry);
    ASSERT(pts);
    return sync_ringbuffer_space_to_read(&pts->buffer) >= 1;
}

bool pty_slave_can_write(dentry_t* dentry, uint32_t start)
{
    pty_slave_entry_t* pts = _pts_get(dentry);
    ASSERT(pts);
    return sync_ringbuffer_space_to_write(&pts->ptm->buffer) >= 0;
}

int pty_slave_read(dentry_t* dentry, uint8_t* buf, uint32_t start, uint32_t len)
{
    pty_slave_entry_t* pts = _pts_get(dentry);
    ASSERT(pts);
    uint32_t leno = sync_ringbuffer_space_to_read(&pts->buffer);
    if (leno > len) {
        leno = len;
    }
    int res = sync_ringbuffer_read(&pts->buffer, buf, leno);
    return leno;
}

int pty_slave_write(dentry_t* dentry, uint8_t* buf, uint32_t start, uint32_t len)
{
    pty_slave_entry_t* pts = _pts_get(dentry);
    ASSERT(pts);
    sync_ringbuffer_write(&pts->ptm->buffer, buf, len);
    return len;
}