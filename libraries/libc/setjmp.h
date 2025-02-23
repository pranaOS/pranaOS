/**
 * @file setjmp.h
 * @author Krisna Pranav
 * @brief setjmp
 * @version 6.0
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <bits/stdint.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

struct __jmp_buf {
#ifdef __i386__
    uint32_t ebx;
    uint32_t esi;
    uint32_t edi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t eip;
#elif __x86_64__
    uint64_t rbx;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
    uint64_t rbp;
    uint64_t rsp;
    uint64_t rip;
#elif __aarch64__
    uint64_t regs[22];
#else
#    error
#endif
    int did_save_signal_mask;
    sigset_t saved_signal_mask;
};

typedef struct __jmp_buf jmp_buf[1];
typedef struct __jmp_buf sigjmp_buf[1];

#ifdef __cplusplus
#    ifdef __i386__
static_assert(sizeof(struct __jmp_buf) == 32, "struct __jmp_buf unsynchronized with i386/setjmp.S");
#    elif __x86_64__
static_assert(sizeof(struct __jmp_buf) == 72, "struct __jmp_buf unsynchronized with x86_64/setjmp.S");
#    elif __aarch64__
static_assert(sizeof(struct __jmp_buf) == 184, "struct __jmp_buf unsynchronized with aarch64/setjmp.S");
#    else
#        error
#    endif
#endif

int setjmp(jmp_buf);
__attribute__((noreturn)) void longjmp(jmp_buf, int val);

int sigsetjmp(sigjmp_buf, int savesigs);
__attribute__((noreturn)) void siglongjmp(sigjmp_buf, int val);

int _setjmp(jmp_buf);
__attribute__((noreturn)) void _longjmp(jmp_buf, int val);

__END_DECLS
