#pragma once

#include <sys/types.h>
#include <kernel/ds/ringbuffer.h>
#include <fcntl.h>

#define BUFFERED (1 << 1)

struct pipe {
    struct ringbuffer *circbuf;
    int pipefd[2];
    flags_t flags;
};  

int pipe_close(vfs_node_t *node);

ssize_t pipe_read_raw(struct pipe *pipe, void *buffer, size_t size, int flags);
ssize_t pipe_write_raw(struct pipe *pipe, const void *buffer, size_t size, int flags);
ssize_t pipe_read(vfs_node_t *node, unsigned int offset, void *buffer,
                  size_t size, int flags);
ssize_t pipe_write(vfs_node_t *node, unsigned int offset, const void *buffer,
                   size_t size, int flags);

int pipe(int pipefd[2]);
