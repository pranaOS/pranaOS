/**
 * @file unistd.h
 * @author Krisna Pranav
 * @brief unistd
 * @version 1.0
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <errno.h>
#include <limits.h>
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

#define HZ 1000
#define STDIN_FILENO 0
#define STDOUT_FILENO 1

extern char** environ;

/**
 * @param buffer 
 * @param buffer_size 
 * @return int 
 */
int get_process_name(char* buffer, int buffer_size);

/**
 * @param name 
 * @param name_length 
 * @return int 
 */
int set_process_name(const char* name, size_t name_length);

/// @brief: dump_backtrace
void dump_backtrace();

/**
 * @param fd 
 * @return int 
 */
int fsync(int fd);

/// @brief: sysbeep
void sysbeep();

/**
 * @return int 
 */
int gettid();

/**
 * @param tid 
 * @return int 
 */
int donate(int tid);

/**
 * @param icon_id 
 * @return int 
 */
int set_process_icon(int icon_id);

/**
 * @return pid_t 
 */
pid_t fork();

/**
 * @param path 
 * @param argv 
 * @return int 
 */
int execv(const char* path, char* const argv[]);

/**
 * @param filename 
 * @param argv 
 * @param envp 
 * @return int 
 */
int execve(const  char* filename, char* const argv[], char* const envp[]);

/**
 * @param path 
 * @param length 
 * @return int 
 */
int truncate(const char* path, off_t length);

/**
 * @return int 
 */
int halt();

/**
 * @return int 
 */
int reboot();

/**
 * @param source_fd 
 * @param target 
 * @param fs_types 
 * @param flags 
 * @return int 
 */
int mount(int source_fd, const char* target, const char* fs_types, int flags);

/**
 * @param mountpoint 
 * @return int 
 */
int unmount(const char* mountpoint);

/**
 * @param promises 
 * @param execpromises 
 * @return int 
 */
int pledge(const char* promises, const char* execpromises);

/**
 * @param path 
 * @param permissions 
 * @return int 
 */
int unveil(const char* path, const char* permissions);

/**
 * @param prompt 
 * @return char* 
 */
char* getpass(const char* prompt);

enum 
{
    _PC_NAME_MAX,
    _PC_PATH_MAX,
    _PC_PIPE_BUF,
    _PC_VDISABLE
};

#define HOST_NAME_MAX 64

#define R_OK 4
#define W_OK 2
#define X_OK 1
#define F_OK 0

__END_DECLS