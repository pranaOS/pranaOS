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


int _tty_process_esc_seq(uint8_t* buf)
{
    if (buf[0] != '\x1b') {
        return 0;
    }
    int argv[4] = { 0, 0, 0, 0 };
    int id = 1;
    int argc = 0;

    if (buf[id] == '[') {
        id++;
        for (;;) {
            int len = 0;
            while ('0' <= buf[id + len] && buf[id + len] <= '9') {
                len++;
            }
            argv[argc++] = stoi(&buf[id], len);
            id += len;
            if (buf[id] != ';') {
                break;
            }
            id++;
        }
    }

    char cmd = buf[id];
    id++;
    switch (cmd) {
    case 'J':
        if (argv[0] == 2) {
        }
        return id;
    case 'H':
        if (argc == 0) {
        }
        if (argc == 2) {
        }
        return id;
    }
    return 0;
}

int tty_write(dentry_t* dentry, uint8_t* buf, uint32_t start, uint32_t len)
{
#ifdef TTY_DEBUG_TIME
    time_t cur_time = timeman_now();
    int secs = cur_time % 60;
    cur_time /= 60;
    int mins = cur_time % 60;
    cur_time /= 60;
    int hrs = cur_time % 24;
    time_t ticks = timeman_get_ticks_from_last_second();
    log_not_formatted("[%d:%d:%d.%d] ", hrs, mins, secs, ticks);
#endif
    for (int i = 0; i < len; i++) {
        if (buf[i] == '\x1b') {
            i += _tty_process_esc_seq(&buf[i]);
        } else {
            log_not_formatted("%c", buf[i]);

        }
    }

    return len;
}

int tty_ioctl(dentry_t* dentry, uint32_t cmd, uint32_t arg)
{
    tty_entry_t* tty = _tty_get(dentry);

    switch (cmd) {
    case TIOCGPGRP:
        return tty->pgid;
    case TIOCSPGRP:
        tty->pgid = arg;
        return 0;
    case TCGETS:
        memcpy((void*)arg, (void*)&tty->termios, sizeof(termios_t));
        return 0;
    case TCSETS:
    case TCSETSW:
    case TCSETSF:
        memcpy((void*)&tty->termios, (void*)arg, sizeof(termios_t));
        if (cmd == TCSETSF) {
            _tty_flush_input(tty);
        }
        return 0;
    }

    return -EINVAL;
}
