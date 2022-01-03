#pragma once

#include <fs/vfs_node.h>
#include <fcntl.h>

#define E_CLOEXEC 1 
#define GLOBAL_FILE_TABLE_SIZE 1024

struct file_lock {
    vfs_node_t *node;
    pid_t pid;
}; 

struct file_descriptor {
    vfs_node_t *node;
    mode_t mode;
    int flags;
    int seek;
    int lock_index;
};

int init_filedescriptors()