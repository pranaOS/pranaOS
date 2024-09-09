/**
 * @file ptrace.h
 * @author Krisna Pranav
 * @brief ptrace
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

#define PT_TRACE_ME 1
#define PT_ATTACH 2
#define PT_CONTINUE 3
#define PT_SYSCALL 4
#define PT_GETREGS 5
#define PT_DETACH 6
#define PT_PEEK 7
#define PT_POKE 8
#define PT_SETREGS 9

#define PT_POKEDEBUG 10
#define PT_PEEKDEBUG 11
#define PT_PEEKBUF 12

#define PT_READ_I PT_PEEK
#define PT_READ_D PT_PEEK
#define PT_WRITE_I PT_POKE
#define PT_WRITE_D PT_POKE

#define DEBUG_STATUS_REGISTER 6
#define DEBUG_CONTROL_REGISTER 7

#ifdef __cplusplus
}
#endif
