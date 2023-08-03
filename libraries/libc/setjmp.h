/**
 * @file setjmp.h
 * @author Krisna Pranav
 * @brief setjmp
 * @version 1.0
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav, 2000-2002 Opsycon AB (www.opsycon.se)
 * 
 */

#pragma once 

#include <signal.h>
#include <stdbool.h>
#include <sys/cdefs.h>
#include <sys/types.h>
#include <bits/stdint.h>

__BEGIN_DECLS

struct __jmp_buf 
{
    uint32_t regs[6];
    bool did_save_signal_mask;
    sigset_t saved_signal_mask;
}; // struct __jmp_buf

/// @brief: jmp_buf, sigjmp_buf
typedef struct __jmp_buf jmp_buf[1];
typedef struct __jmp_buf sigjmp_buf[1];

/**
 * @return int 
 */
int setjmp(jmp_buf);

/**
 * @param val 
 */
void longjmp(jmp_buf, int val);

/**
 * @param savesigs 
 * @return int 
 */
int sigsetjmp(sigjmp_buf, int savesigs);

/**
 * @param val 
 */
void siglongjmp(sigjmp_buf, int val);

__END_DECLS