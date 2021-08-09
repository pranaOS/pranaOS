/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <fs/procfs/procfs.h>
#include <fs/vfs.h>
#include <libkernel/bits/errno.h>
#include <libkernel/libkernel.h>
#include <libkernel/log.h>


extern const file_ops_t procfs_root_ops;
int procfs_read_inode(dentry_t* dentry)
{
    if (dentry->inode_indx != 2) {
#ifdef PROCFS_DEBUG
        log_warn("NOT ROOT ENTRY ID READ IN PROCFS");
#endif
        return -1;
    }
    procfs_inode_t* procfs_inode = (procfs_inode_t*)dentry->inode;
    memset((void*)procfs_inode, 0, sizeof(procfs_inode_t));
    procfs_inode->index = 2;
    procfs_inode->mode = S_IFDIR;
    procfs_inode->ops = &procfs_root_ops;
    return 0;
}

int procfs_write_inode(dentry_t* dentry)
{
    return 0;
}

int procfs_free_inode(dentry_t* dentry)
{
    return 0;
}

fsdata_t procfs_data(dentry_t* dentry)
{
    fsdata_t fsdata;
    fsdata.sb = 0;
    fsdata.gt = 0;
    return fsdata;
}
