/**
 * @file regs.h
 * @author Krisna Pranav
 * @brief registers 
 * @version 6.0
 * @date 2023-07-05
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/platform.h>

#if defined(__cplusplus) && defined(__cpp_concepts)
#    include <mods/types.h>
#else
#    include <sys/types.h>
#endif

struct [[gnu::packed]] PtraceRegisters 
{
#if ARCH(I386)
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t eip;
    uint32_t eflags;
#else
    uint64_t rax;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rbx;
    uint64_t rsp;
    uint64_t rbp;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rip;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
    uint64_t rflags;
#endif
    uint32_t cs;
    uint32_t ss;
    uint32_t ds;
    uint32_t es;
    uint32_t fs;
    uint32_t gs;

#if defined(__cplusplus) && defined(__cpp_concepts)
    FlatPtr ip() const
    {
#    if ARCH(I386)
        return eip;
#    else
        return rip;
#    endif
    }

    void set_ip(FlatPtr ip)
    {
#    if ARCH(I386)
        eip = ip;
#    else
        rip = ip;
#    endif
    }

    FlatPtr bp() const
    {
#    if ARCH(I386)
        return ebp;
#    else
        return rbp;
#    endif
    }

    void set_bp(FlatPtr bp)
    {
#    if ARCH(I386)
        ebp = bp;
#    else
        rbp = bp;
#    endif
    }
#endif
}; // struct [[gnu::packed]] PtraceRegisters 