#pragma once

#include <fs/vfs_node.h>

vfs_node_t *chardev_create(char *path, read_fpointer read, write_fpointer write,
                           ioctl_fpointer cmd);