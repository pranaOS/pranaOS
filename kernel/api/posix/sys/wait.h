/**
 * @file wait.h
 * @author Krisna Pranav
 * @brief wait
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

#define WEXITSTATUS(status) (((status)&0xff00) >> 8)
#define WSTOPSIG(status) WEXITSTATUS(status)
#define WTERMSIG(status) ((status)&0x7f)
#define WIFEXITED(status) (WTERMSIG(status) == 0)
#define WIFSTOPPED(status) (((status)&0xff) == 0x7f)
#define WIFSIGNALED(status) (((char)(((status)&0x7f) + 1) >> 1) > 0)

#define WNOHANG 1
#define WUNTRACED 2
#define WSTOPPED WUNTRACED
#define WEXITED 4
#define WCONTINUED 8
#define WNOWAIT 0x1000000

typedef enum 
{
    P_ALL = 1,
    P_PID,
    P_PGID
} idtype_t;

#ifdef __cplusplus
}
#endif
