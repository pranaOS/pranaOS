/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


// includes
#include <drivers/x86/keyboard.h>
#include <fs/devfs/devfs.h>
#include <fs/vfs.h>
#include <io/tty/tty.h>
#include <libkernel/bits/errno.h>
#include <libkernel/libkernel.h>
#include <libkernel/log.h>
#include <libkernel/syscall_structs.h>
#include <mem/kmalloc.h>
#include <tasking/signal.h>
#include <tasking/tasking.h>

static int next_tty = 0;
static tty_entry_t* active_tty = 0;
tty_entry_t ttys[TTY_MAX_COUNT];

static tty_entry_t* _tty_get(dentry_t* dentry)
{
    for (int i = 0; i < TTY_MAX_COUNT; i++) {
        if (dentry->inode_indx == ttys[i].inode_indx) {
            return &ttys[i];
        }
    }
    return 0;
}

static inline void _tty_flush_input(tty_entry_t* tty)
{
    tty->lines_avail = 0;
    sync_ringbuffer_clear(&tty->buffer);
}

inline static tty_entry_t* _tty_active()
{
    return active_tty;
}

bool tty_can_read(dentry_t* dentry, uint32_t start)
{
    tty_entry_t* tty = _tty_get(dentry);
    if (tty->termios.c_lflag & ICANON) {
        return tty->lines_avail > 0;
    }
    return sync_ringbuffer_space_to_read(&tty->buffer) >= 1;
}

bool tty_can_write(dentry_t* dentry, uint32_t start)
{
    return true;
}

int tty_read(dentry_t* dentry, uint8_t* buf, uint32_t start, uint32_t len)
{
    tty_entry_t* tty = _tty_get(dentry);
    uint32_t leno = sync_ringbuffer_space_to_read(&tty->buffer);
    if (leno > len) {
        leno = len;
    }
    int res = sync_ringbuffer_read(&tty->buffer, buf, leno);
    if (tty->termios.c_lflag & ICANON && res == leno) {
        tty->lines_avail--;
    }
    return leno;
}