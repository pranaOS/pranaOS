/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

//#ifndef _LIBC_UNISTD_H
//#define _LIBC_UNISTD_H

#pragma once

// includes
#include <fcntl.h>
#include <stddef.h>
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

int fork();
int execve(const char* path, char** argv, char** env);
int wait(int pid);
pid_t getpid();
int setpgid(pid_t cmd, pid_t arg);
pid_t getpgid(pid_t arg);

int close(int fd);
ssize_t read(int fd, char* buf, size_t count);
ssize_t write(int fd, const void* buf, size_t count);
int rmdir(const char* path);
int chdir(const char* path);
int unlink(const char* path);
off_t lseek(int fd, off_t off, int whence);

uid_t getuid();
char* getlogin();

int nice(int inc);

__END_DECLS

//#endif
