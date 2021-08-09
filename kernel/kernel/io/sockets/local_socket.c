/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <io/sockets/local_socket.h>
#include <libkernel/bits/errno.h>
#include <libkernel/libkernel.h>
#include <libkernel/log.h>
#include <mem/kmalloc.h>
#include <tasking/proc.h>
#include <tasking/tasking.h>


static file_ops_t local_socket_ops = {
    .can_read = local_socket_can_read,
    .can_write = local_socket_can_write,
    .read = local_socket_read,
    .write = local_socket_write,
    .open = 0,
    .truncate = 0,
    .create = 0,
    .unlink = 0,
    .getdents = 0,
    .lookup = 0,
    .mkdir = 0,
    .rmdir = 0,
    .fstat = 0,
    .ioctl = 0,
    .mmap = 0,
};

int local_socket_create(int type, int protocol, file_descriptor_t* fd)
{
    return socket_create(PF_LOCAL, type, protocol, fd, &local_socket_ops);
}

bool local_socket_can_read(dentry_t* dentry, uint32_t start)
{
    socket_t* sock_entry = (socket_t*)dentry;
    return sync_ringbuffer_space_to_read_with_custom_start(&sock_entry->buffer, start) != 0;
}

int local_socket_read(dentry_t* dentry, uint8_t* buf, uint32_t start, uint32_t len)
{
    socket_t* sock_entry = (socket_t*)dentry;
    uint32_t read = sync_ringbuffer_read_with_start(&sock_entry->buffer, start, buf, len);
    return read;
}

bool local_socket_can_write(dentry_t* dentry, uint32_t start)
{
    return true;
}

int local_socket_write(dentry_t* dentry, uint8_t* buf, uint32_t start, uint32_t len)
{
    socket_t* sock_entry = (socket_t*)dentry;
    uint32_t written = sync_ringbuffer_write_ignore_bounds(&sock_entry->buffer, buf, len);
    return 0;
}
