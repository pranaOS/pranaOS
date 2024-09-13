/**
 * @file registerstate.h
 * @author Krisna Pranav
 * @brief Register State
 * @version 6.0
 * @date 2024-09-13
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>
#include <mods/platform.h>
#include <libc/sys/arch/i386/regs.h>
#include <kernel/arch/x86/asm_wrapper.h>
#include <kernel/arch/x86/cpu.h>


VALIDATE_IS_X86()

namespace Kernel 
{
    struct [[gnu::packed]] RegisterState 
    {
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

    #if ARCH(I386)
        /**
         * @return FlatPtr 
         */
        FlatPtr userspace_sp() const
        {
            return userspace_esp;
        }

        /**
         * @brief Set the userspace sp object
         * 
         * @param value 
         */
        void set_userspace_sp(FlatPtr value) 
        { 
            userspace_esp = value; 
        }

        /**
         * @return FlatPtr 
         */
        FlatPtr ip() const 
        { 
            return eip; 
        }

        /**
         * @brief Set the ip object
         * 
         * @param value 
         */
        void set_ip(FlatPtr value) 
        { 
            eip = value; 
        }

        /**
         * @brief Set the dx object
         * 
         * @param value 
         */
        void set_dx(FlatPtr value) 
        { 
            edx = value; 
        }

        /**
         * @return FlatPtr 
         */
        FlatPtr bp() const 
        { 
            return ebp; 
        }

        /**
         * @brief Set the bp object
         * 
         * @param value 
         */
        void set_bp(FlatPtr value) 
        { 
            ebp = value; 
        }

        /**
         * @return FlatPtr 
         */
        FlatPtr flags() const 
        { 
            return eflags; 
        }

        /**
         * @brief Set the flags object
         * 
         * @param value 
         */
        void set_flags(FlatPtr value) 
        { 
            eflags = value; 
        }

        /**
         * @brief Set the return reg object
         * 
         * @param value 
         */
        void set_return_reg(FlatPtr value) 
        { 
            eax = value; 
        }
    #elif ARCH(X86_64)
        /**
         * @return FlatPtr 
         */
        FlatPtr userspace_sp() const
        {
            return userspace_rsp;
        }

        /**
         * @brief Set the userspace sp object
         * 
         * @param value 
         */
        void set_userspace_sp(FlatPtr value) 
        { 
            userspace_rsp = value; 
        }

        /**
         * @return FlatPtr 
         */
        FlatPtr ip() const 
        { 
            return rip; 
        }

        /**
         * @brief Set the ip object
         * 
         * @param value 
         */
        void set_ip(FlatPtr value) 
        { 
            rip = value; 
        }

        /**
         * @brief Set the dx object
         * 
         * @param value 
         */
        void set_dx(FlatPtr value) 
        { 
            rdx = value; 
        }

        /**
         * @return FlatPtr 
         */
        FlatPtr bp() const 
        { 
            return rbp; 
        }

        /**
         * @brief Set the bp object
         * 
         * @param value 
         */
        void set_bp(FlatPtr value) 
        { 
            rbp = value; 
        }

        /**
         * @return FlatPtr 
         */
        FlatPtr flags() const 
        { 
            return rflags; 
        }

        /**
         * @brief Set the flags object
         * 
         * @param value 
         */
        void set_flags(FlatPtr value) 
        { 
            rflags = value; 
        }

        /**
         * @brief Set the return reg object
         * 
         * @param value 
         */
        void set_return_reg(FlatPtr value) 
        { 
            rax = value; 
        }
    #endif

        /**
         * @param function 
         * @param arg1 
         * @param arg2 
         * @param arg3 
         * @param arg4 
         */
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
            arg2 = rdi;
            arg3 = rbx;
            arg4 = rsi;
    #endif
        }
    }; // struct [[gnu::packed]] RegisterState 

    #if ARCH(I386)
    #    define REGISTER_STATE_SIZE (19 * 4)
    static_assert(AssertSize<RegisterState, REGISTER_STATE_SIZE>());
    #elif ARCH(X86_64)
    #    define REGISTER_STATE_SIZE (22 * 8)
    static_assert(AssertSize<RegisterState, REGISTER_STATE_SIZE>());
    #endif

    /**
     * @param ptrace_regs 
     * @param kernel_regs 
     */
    inline void copy_kernel_registers_into_ptrace_registers(PtraceRegisters& ptrace_regs, const RegisterState& kernel_regs)
    {
    #if ARCH(I386)
        ptrace_regs.eax = kernel_regs.eax,
        ptrace_regs.ecx = kernel_regs.ecx,
        ptrace_regs.edx = kernel_regs.edx,
        ptrace_regs.ebx = kernel_regs.ebx,
        ptrace_regs.esp = kernel_regs.userspace_esp,
        ptrace_regs.ebp = kernel_regs.ebp,
        ptrace_regs.esi = kernel_regs.esi,
        ptrace_regs.edi = kernel_regs.edi,
        ptrace_regs.eip = kernel_regs.eip,
        ptrace_regs.eflags = kernel_regs.eflags,
    #else
        ptrace_regs.rax = kernel_regs.rax,
        ptrace_regs.rcx = kernel_regs.rcx,
        ptrace_regs.rdx = kernel_regs.rdx,
        ptrace_regs.rbx = kernel_regs.rbx,
        ptrace_regs.rsp = kernel_regs.userspace_rsp,
        ptrace_regs.rbp = kernel_regs.rbp,
        ptrace_regs.rsi = kernel_regs.rsi,
        ptrace_regs.rdi = kernel_regs.rdi,
        ptrace_regs.rip = kernel_regs.rip,
        ptrace_regs.r8 = kernel_regs.r8;
        ptrace_regs.r9 = kernel_regs.r9;
        ptrace_regs.r10 = kernel_regs.r10;
        ptrace_regs.r11 = kernel_regs.r11;
        ptrace_regs.r12 = kernel_regs.r12;
        ptrace_regs.r13 = kernel_regs.r13;
        ptrace_regs.r14 = kernel_regs.r14;
        ptrace_regs.r15 = kernel_regs.r15;
        ptrace_regs.rflags = kernel_regs.rflags,
    #endif
        ptrace_regs.cs = 0;
        ptrace_regs.ss = 0;
        ptrace_regs.ds = 0;
        ptrace_regs.es = 0;
        ptrace_regs.fs = 0;
        ptrace_regs.gs = 0;
    }

    /**
     * @param kernel_regs 
     * @param ptrace_regs 
     */
    inline void copy_ptrace_registers_into_kernel_registers(RegisterState& kernel_regs, const PtraceRegisters& ptrace_regs)
    {
    #if ARCH(I386)
        kernel_regs.eax = ptrace_regs.eax;
        kernel_regs.ecx = ptrace_regs.ecx;
        kernel_regs.edx = ptrace_regs.edx;
        kernel_regs.ebx = ptrace_regs.ebx;
        kernel_regs.esp = ptrace_regs.esp;
        kernel_regs.ebp = ptrace_regs.ebp;
        kernel_regs.esi = ptrace_regs.esi;
        kernel_regs.edi = ptrace_regs.edi;
        kernel_regs.eip = ptrace_regs.eip;
        kernel_regs.eflags = (kernel_regs.eflags & ~safe_eflags_mask) | (ptrace_regs.eflags & safe_eflags_mask);
    #else
        kernel_regs.rax = ptrace_regs.rax;
        kernel_regs.rcx = ptrace_regs.rcx;
        kernel_regs.rdx = ptrace_regs.rdx;
        kernel_regs.rbx = ptrace_regs.rbx;
        kernel_regs.rsp = ptrace_regs.rsp;
        kernel_regs.rbp = ptrace_regs.rbp;
        kernel_regs.rsi = ptrace_regs.rsi;
        kernel_regs.rdi = ptrace_regs.rdi;
        kernel_regs.rip = ptrace_regs.rip;
        kernel_regs.r8 = ptrace_regs.r8;
        kernel_regs.r9 = ptrace_regs.r9;
        kernel_regs.r10 = ptrace_regs.r10;
        kernel_regs.r11 = ptrace_regs.r11;
        kernel_regs.r12 = ptrace_regs.r12;
        kernel_regs.r13 = ptrace_regs.r13;
        kernel_regs.r14 = ptrace_regs.r14;
        kernel_regs.r15 = ptrace_regs.r15;
        kernel_regs.rflags = (kernel_regs.rflags & ~safe_eflags_mask) | (ptrace_regs.rflags & safe_eflags_mask);
    #endif
    }

    struct [[gnu::packed]] DebugRegisterState 
    {
        FlatPtr dr0;
        FlatPtr dr1;
        FlatPtr dr2;
        FlatPtr dr3;
        FlatPtr dr6;
        FlatPtr dr7;
    }; // struct [[gnu::packed]] DebugRegisterState 

    /**
     * @param state 
     */
    inline void read_debug_registers_into(DebugRegisterState& state)
    {
        state.dr0 = read_dr0();
        state.dr1 = read_dr1();
        state.dr2 = read_dr2();
        state.dr3 = read_dr3();
        state.dr6 = read_dr6();
        state.dr7 = read_dr7();
    }

    /**
     * @param state 
     */
    inline void write_debug_registers_from(const DebugRegisterState& state)
    {
        write_dr0(state.dr0);
        write_dr1(state.dr1);
        write_dr2(state.dr2);
        write_dr3(state.dr3);
        write_dr6(state.dr6);
        write_dr7(state.dr7);
    }

    inline void clear_debug_registers()
    {
        write_dr0(0);
        write_dr1(0);
        write_dr2(0);
        write_dr3(0);
        write_dr7(1 << 10); 
    }
} // namespace Kernel