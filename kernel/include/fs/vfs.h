/*
 * Copyright (c) 2021-2022 krishpranav <krisna.pranav@gmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <fs/vfs_node.h>
#include <sys/stat.h>
#include <unistd.h>

struct socket;
struct socket_conn;

extern vfs_node_t *vfs_root;

struct file
{
	vfs_node_t *vfs_node;
	int         fd;
	size_t      filesize;
};

int init_vfs();

ssize_t vfs_read_fd(int fd, void *buf, size_t amount);

ssize_t vfs_read(struct file *file, void *buf, size_t amount);

ssize_t vfs_write_fd(int fd, const void *buf, size_t amount);

ssize_t vfs_write(struct file *file, const void *buf, size_t amount);

int vfs_open_fd(const char *path, int flags, int mode);

struct file *vfs_open(const char *path, int flags, int mode);

int vfs_creat(const char *path, int mode);

int vfs_close(struct file *file);

int vfs_close_fd(int fd);

struct dirent *vfs_readdir(DIR *dirstream);

DIR *vfs_opendir(const char *filepath);

vfs_node_t *vfs_find_path(const char *path);

vfs_node_t *vfs_setupnode(char *name, uint8_t type, uint16_t permissions,
                          uid_t uid, gid_t gid, size_t size, offset_t offset,
                          open_fpointer open, close_fpointer close,
                          creat_fpointer creat, read_fpointer read,
                          write_fpointer write, open_dir_fpointer opendir,
                          read_dir_fpointer readdir, filesystem_t *fs_info);

int vfs_check_if_initialised();

int vfs_link_node_vfs(const char *path, vfs_node_t *node);

char *vfs_get_name(const char *path);

int vfs_fstat(int fd, struct stat *statbuf);
int vfs_stat(const char *pathname, struct stat *statbuf);

off_t vfs_lseek(int fd, off_t offset, int whence);
void  vfs_interpret_path(char *buf);

vfs_node_t *vfs_sock_create_conn(struct socket_conn *socket);
int         vfs_sock_create(char *path, struct socket *socket);
char *      vfs_fullpath(char *path);