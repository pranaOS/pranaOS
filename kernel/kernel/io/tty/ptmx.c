/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <fs/devfs/devfs.h>
#include <io/tty/ptmx.h>
#include <io/tty/pty_master.h>
#include <libkernel/libkernel.h>
#include <libkernel/log.h>

#define PTY_DEBUG

int ptmx_write(dentry_t* dentry, uint8_t* buf, uint32_t start, uint32_t len)
{
    return 0;
}

int ptmx_open(dentry_t* dentry, struct file_descriptor* fd, uint32_t flags)
{
#ifndef PTY_DEBUG
    log("OPENING PTMX");
#endif  
    return pty_master_alloc(fd);
}

int ptmx_install()
{
    dentry_t* mp
    if (vfs_resolve_path("/dev", &mp) < 0) {
        return -1;
    }

    files_opts_t fops = { 0 };
    fops.open = ptmx_open;
    dentry_put(mp);
    return 0;
}