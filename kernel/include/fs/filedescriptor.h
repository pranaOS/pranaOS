#pragma once

#include <fs/vfs_node.h>
#include <fcntl.h>

#define E_CLOEXEC 1
#define GLOBAL_FILE_TABLE_SIZE 1024

struct file_lock
{
	vfs_node_t *node; 
	pid_t       pid;  
};

struct file_descriptor
{
	vfs_node_t *node;
	mode_t      mode;
	int         flags;
	int         seek;
	int         lock_index;
};

int                     init_filedescriptors();
int                     close_filedescriptor(int fd);
struct file_descriptor *get_filedescriptor(int fd);
struct file_descriptor *get_filedescriptor_from_node(vfs_node_t *node);
vfs_node_t *            get_filedescriptor_node(int fd);
int                     register_filedescriptor(vfs_node_t *node, int flags, int mode);
void                    check_filedescriptors();
int                     setflags_filedescriptor(int fd, int flags);
int                     dup_filedescriptor(int fd, int from);
int                     dup2_filedescriptor(int old, int new);
void                    debug_filedescriptors();