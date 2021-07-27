/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Types.h>
#include <libc/sys/arch/i386/regs.h>
#include <kernel/Arch/x86/ASM_wrapper.h>
#include <kernel/Arch/x86/CPU.h>

namespace Kernel {

struct [[gnu::packed]] RegisterState {
#if ARCH(I386)
    FlatPtr ss;
    FlatPtr gs;
    FlatPtr fs;
    FlatPtr es;
    FlatPtr ds;
    FlatPtr edi;
    FlatPtr esi;
    FlatPtr ebp;
    FlatPtr esp;
    FlatPtr ebx;
    FlatPtr edx;
    FlatPtr ecx;
    FlatPtr eax;
#else
    FlatPtr rdi;
    FlatPtr rsi;
    FlatPtr rbp;
    FlatPtr rsp;
    FlatPtr rbx;
    FlatPtr rdx;
    FlatPtr rcx;
    FlatPtr rax;
    FlatPtr r8;
    FlatPtr r9;
    FlatPtr r10;
    FlatPtr r11;
    FlatPtr r12;
    FlatPtr r13;
    FlatPtr r14;
    FlatPtr r15;
#endif
    u16 exception_code;
    u16 isr_number;
#if ARCH(X86_64)
    u32 padding;
#endif
#if ARCH(I386)
    FlatPtr eip;
#else
    FlatPtr rip;
#endif
    FlatPtr cs;
#if ARCH(I386)
    FlatPtr eflags;
    FlatPtr userspace_esp;
    FlatPtr userspace_ss;
#else
    FlatPtr rflags;
    FlatPtr userspace_rsp;
    FlatPtr userspace_ss;
#endif

    FlatPtr userspace_sp() const
    {
#if ARCH(I386)
        return userspace_esp;
#else
        return userspace_rsp;
#endif
    }

    FlatPtr ip() const
    {
#if ARCH(I386)
        return eip;
#else
        return rip;
#endif
    }

    FlatPtr bp() const
    {
#if ARCH(I386)
        return ebp;
#else
        return rbp;
#endif
    }

    FlatPtr flags() const
    {
#if ARCH(I386)
        return eflags;
#else
        return rflags;
#endif
    }

    void capture_syscall_params(FlatPtr& function, FlatPtr& arg1, FlatPtr& arg2, FlatPtr& arg3, FlatPtr& arg4) const
    {
#if ARCH(I386)
        function = eax;
        arg1 = edx;
        arg2 = ecx;
        arg3 = ebx;
        arg4 = esi;
#else
        function = rax;
        arg1 = rdx;
        arg2 = rcx;
        arg3 = rbx;
        arg4 = rsi;
#endif
    }

    void set_ip_reg(FlatPtr value)
    {
#if ARCH(I386)
        eip = value;
#else
        rip = value;
#endif
    }

    void set_return_reg(FlatPtr value)
    {
#if ARCH(I386)
        eax = value;
#else
        rax = value;
#endif
    }
};

#if ARCH(I386)
#    define REGISTER_STATE_SIZE (19 * 4)
#else
#    define REGISTER_STATE_SIZE (22 * 8)
#endif
static_assert(REGISTER_STATE_SIZE == sizeof(RegisterState));

}