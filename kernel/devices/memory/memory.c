/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <filesystem/cdev.h>
#include <errno/error.h>
#include <debug/debug.h>
#include <libkernel/math.h>

#define MEMORY_MAJOR 1
#define NULL_DEVICE 3
#define RANDOM_DEVICE 8

extern struct vfs_file_operations def_chr_fops;

static int
null_open(struct vfs_inode *inode, struct vfs_file *filp) {
    return 0;
}

static int
null_release(struct vfs_inode *inode, struct vfs_file *filp) {
    return 0;
}

static loff_t
null_llseek(struct vfs_file *file, loff_t ppos, int whence) {
    return 0;
}

static ssize_t
null_read(struct vfs_file *file, char *buf, size_t count, loff_t ppos) {
    return 0;
}

static ssize_t
null_write(struct vfs_file *file, const char *buf, size_t count, loff_t ppos) {
    return 0;
}

static struct 
vfs_file_operations null_fops = {
    .llseek = null_llseek,
    .read = null_read,
    .write = null_write,
    .open = null_open,
    .release = null_release,
};

static int
random_open(struct vfs_inode *inode, struct vfs_file *filp) {
    return 0;
}

static int
random_release(struct vfs_inode *inode, struct vfs_file *filp) {
    return 0;
}