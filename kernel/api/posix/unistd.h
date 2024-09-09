/**
 * @file unistd.h
 * @author Krisna Pranav
 * @brief unistd
 * @version 6.0
 * @date 2024-09-09
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/api/posix/sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#define R_OK 4
#define W_OK 2
#define X_OK 1
#define F_OK 0

#define MS_NODEV (1 << 0)
#define MS_NOEXEC (1 << 1)
#define MS_NOSUID (1 << 2)
#define MS_BIND (1 << 3)
#define MS_RDONLY (1 << 4)
#define MS_REMOUNT (1 << 5)

enum 
{
    _SC_MONOTONIC_CLOCK,
    _SC_NPROCESSORS_CONF,
    _SC_NPROCESSORS_ONLN,
    _SC_OPEN_MAX,
    _SC_HOST_NAME_MAX,
    _SC_TTY_NAME_MAX,
    _SC_PAGESIZE,
    _SC_GETPW_R_SIZE_MAX,
    _SC_CLK_TCK,
    _SC_SYMLOOP_MAX,
    _SC_MAPPED_FILES,
    _SC_ARG_MAX,
};

#define _SC_MONOTONIC_CLOCK _SC_MONOTONIC_CLOCK
#define _SC_NPROCESSORS_CONF _SC_NPROCESSORS_CONF
#define _SC_NPROCESSORS_ONLN _SC_NPROCESSORS_ONLN
#define _SC_OPEN_MAX _SC_OPEN_MAX
#define _SC_PAGESIZE _SC_PAGESIZE
#define _SC_HOST_NAME_MAX _SC_HOST_NAME_MAX
#define _SC_TTY_NAME_MAX _SC_TTY_NAME_MAX
#define _SC_GETPW_R_SIZE_MAX _SC_GETPW_R_SIZE_MAX
#define _SC_CLK_TCK _SC_CLK_TCK
#define _SC_SYMLOOP_MAX _SC_SYMLOOP_MAX
#define _SC_MAPPED_FILES _SC_MAPPED_FILES
#define _SC_ARG_MAX _SC_ARG_MAX

#ifdef __cplusplus
}
#endif


