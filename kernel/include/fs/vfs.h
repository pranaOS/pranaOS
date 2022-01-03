#pragma once

#include <fs/vfs_node.h>
#include <sys/stat.h>
#include <unistd.h>

struct socket;
struct socket_conn;

extern vfs_node_t *vfs_root;

struct file {
    vfs_node_t *vfs_node;
    int fd;
    size_t filesize;
};

int init_vfs();

ssize_t vfs_read_fd(int fd, void *buf, size_t amount);

ssize_t vfs_read(struct file *file, void *buf, size_t amount);

ssize_t vfs_write_fd(int fd, const void *buf, size_t amount);

ssize_t vfs_write(struct file *file, const void *buf, size_t amount);