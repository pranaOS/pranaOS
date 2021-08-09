/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <algo/sync_ringbuffer.h>
#include <io/sockets/socket.h>
#include <libkernel/kassert.h>

socket_t socket_list[MAX_SOCKET_COUNT];
static int next_socket = 0;

static socket_t* _socket_create(int domain, int type, int protocol)
{
    socket_list[next_socket].domain = domain;
    socket_list[next_socket].type = type;
    socket_list[next_socket].protocol = protocol;
    socket_list[next_socket].buffer = sync_ringbuffer_create_std();
    socket_list[next_socket].d_count = 1;
    lock_init(&socket_list[next_socket].lock);
    return &socket_list[next_socket++];
}


int socket_create(int domain, int type, int protocol, file_descriptor_t* fd, file_ops_t* ops)
{
    fd->type = FD_TYPE_SOCKET;
    fd->sock_entry = _socket_create(domain, type, protocol);
    if (!fd->sock_entry) {
        return -1;
    }
    return 0;
}

int socket_put(socket_t* sock)
{
    lock_acquire(&sock->lock);
    sock->d_count--;
    if (sock->d_count == 0) {
        sync_ringbuffer_free(&sock);
    }
    lock_release(&sock);
    return 0;
}