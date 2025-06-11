/**
 * @file sandbox.h
 * @author Krisna Pranav
 * @brief sandbox
 * @version 6.0
 * @date 2025-06-10
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

typedef boolean (*sb_syscall_handler)(u64 arg0, u64 arg1, u64 arg2, u64 arg3, u64 arg4, u64 arg5,
    sysreturn *rv);

typedef struct sb_syscall {
    struct buffer sb_handlers;
    sysreturn (*default_handler)(u64 arg0, u64 arg1, u64 arg2, u64 arg3, u64 arg4, u64 arg5);
} *sb_syscall;

#define SYS_pledge  335
#define SYS_unveil  336

/**
 * @param syscalls 
 * @param cfg 
 * @return boolean 
 */
boolean pledge_init(sb_syscall syscalls, tuple cfg);

/**
 * @param syscalls 
 * @param cfg 
 * @return boolean 
 */
boolean unveil_init(sb_syscall syscalls, tuple cfg);