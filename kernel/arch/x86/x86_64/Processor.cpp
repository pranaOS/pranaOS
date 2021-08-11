/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/StdLibExtras.h>
#include <kernel/arch/x86/Processor.h>
#include <kernel/arch/x86/TrapFrame.h>
#include <kernel/Panic.h>
#include <kernel/Process.h>
#include <kernel/Random.h>
#include <kernel/Sections.h>
#include <kernel/Thread.h>

namespace Kernel {

NAKED void thread_context_first_enter(void)
{

    asm(

        "    popq %rdi \n" 
        "    popq %rsi \n" 
        "    popq %rdx \n"
        "    cld \n"
        "    call context_first_init \n"
        "    jmp common_trap_exit \n");
};

NAKED void do_assume_context(Thread*, u32)
{

    asm(
        "    movq %rdi, %r12 \n" 
        "    movq %rsi, %r13 \n" 
        
        "    subq $(" __STRINGIFY(16 + REGISTER_STATE_SIZE + TRAP_FRAME_SIZE + 8) "), %rsp \n"
        "    cld \n"
        "    call do_init_context \n"
        "    movq %rax, %rsp \n"                          
        "    movq %r12, %rdi \n"                          
        "    movq %r12, %rsi \n"                          
        "    pushq %r12 \n"                               
        "    pushq %r12 \n"                               
        "    leaq thread_context_first_enter(%rip), %r12 \n" 
        "    pushq %r12 \n"
        "    jmp enter_thread_context \n");
}

String Processor::platform_string() const
{
    return "x86_64";
}

FlatPtr Processor::init_context(Thread& thread, bool leave_crit)
{
    VERIFY(is_kernel_mode());
    VERIFY(g_scheduler_lock.is_locked());
    if (leave_crit) {

        VERIFY(in_critical() == 2);
        m_in_critical = 1; 
    }

    u64 kernel_stack_top = thread.kernel_stack_top();

    kernel_stack_top -= round_up_to_power_of_two(get_fast_random<u8>(), 16);

    u64 stack_top = kernel_stack_top;

    VERIFY((cpu_flags() & 0x24000) == 0); 

    auto& regs = thread.regs();
    bool return_to_user = (regs.cs & 3) != 0;

    stack_top -= 1 * sizeof(u64);
    *reinterpret_cast<u64*>(kernel_stack_top - 2 * sizeof(u64)) = FlatPtr(&exit_kernel_thread);

    stack_top -= sizeof(RegisterState);

    stack_top -= sizeof(u64);
    *reinterpret_cast<u64*>(kernel_stack_top - sizeof(u64)) = 0;

    RegisterState& iretframe = *reinterpret_cast<RegisterState*>(stack_top);
    iretframe.rdi = regs.rdi;
    iretframe.rsi = regs.rsi;
    iretframe.rbp = regs.rbp;
    iretframe.rsp = 0;
    iretframe.rbx = regs.rbx;
    iretframe.rdx = regs.rdx;
    iretframe.rcx = regs.rcx;
    iretframe.rax = regs.rax;
    iretframe.r8 = regs.r8;
    iretframe.r9 = regs.r9;
    iretframe.r10 = regs.r10;
    iretframe.r11 = regs.r11;
    iretframe.r12 = regs.r12;
    iretframe.r13 = regs.r13;
    iretframe.r14 = regs.r14;
    iretframe.r15 = regs.r15;
    iretframe.rflags = regs.rflags;
    iretframe.rip = regs.rip;
    iretframe.cs = regs.cs;
    if (return_to_user) {
        iretframe.userspace_rsp = regs.rsp;
        iretframe.userspace_ss = GDT_SELECTOR_DATA3 | 3;
    } else {
        iretframe.userspace_rsp = kernel_stack_top;
        iretframe.userspace_ss = 0;
    }

    stack_top -= sizeof(TrapFrame);
    TrapFrame& trap = *reinterpret_cast<TrapFrame*>(stack_top);
    trap.regs = &iretframe;
    trap.prev_irq_level = 0;
    trap.next_trap = nullptr;

    stack_top -= sizeof(u64); 
    *reinterpret_cast<u64*>(stack_top) = stack_top + 8;

    if constexpr (CONTEXT_SWITCH_DEBUG) {
        if (return_to_user) {
            dbgln("init_context {} ({}) set up to execute at rip={}:{}, rsp={}, stack_top={}, user_top={}",
                thread,
                VirtualAddress(&thread),
                iretframe.cs, regs.rip,
                VirtualAddress(regs.rsp),
                VirtualAddress(stack_top),
                iretframe.userspace_rsp);
        } else {
            dbgln("init_context {} ({}) set up to execute at rip={}:{}, rsp={}, stack_top={}",
                thread,
                VirtualAddress(&thread),
                iretframe.cs, regs.rip,
                VirtualAddress(regs.rsp),
                VirtualAddress(stack_top));
        }
    }

    regs.rip = FlatPtr(&thread_context_first_enter);
    regs.rsp0 = kernel_stack_top;
    regs.rsp = stack_top;
    return stack_top;
}

void Processor::switch_context(Thread*& from_thread, Thread*& to_thread)
{
    VERIFY(!in_irq());
    VERIFY(m_in_critical == 1);
    VERIFY(is_kernel_mode());

    dbgln_if(CONTEXT_SWITCH_DEBUG, "switch_context --> switching out of: {} {}", VirtualAddress(from_thread), *from_thread);

    // m_in_critical is restored in enter_thread_context
    from_thread->save_critical(m_in_critical);

    asm volatile(
        
        "pushfq \n"
        "pushq %%rbx \n"
        "pushq %%rcx \n"
        "pushq %%rbp \n"
        "pushq %%rsi \n"
        "pushq %%rdi \n"
        "pushq %%r8 \n"
        "pushq %%r9 \n"
        "pushq %%r10 \n"
        "pushq %%r11 \n"
        "pushq %%r12 \n"
        "pushq %%r13 \n"
        "pushq %%r14 \n"
        "pushq %%r15 \n"
        "movq %%rsp, %[from_rsp] \n"
        "leaq 1f(%%rip), %%rbx \n"
        "movq %%rbx, %[from_rip] \n"
        "movq %[to_rsp0], %%rbx \n"
        "movl %%ebx, %[tss_rsp0l] \n"
        "shrq $32, %%rbx \n"
        "movl %%ebx, %[tss_rsp0h] \n"
        "movq %[to_rsp], %%rsp \n"
        "pushq %[to_thread] \n"
        "pushq %[from_thread] \n"
        "pushq %[to_rip] \n"
        "cld \n"
        "movq 16(%%rsp), %%rsi \n"
        "movq 8(%%rsp), %%rdi \n"
        "jmp enter_thread_context \n"
        "1: \n"
        "popq %%rdx \n"
        "popq %%rax \n"
        "popq %%r15 \n"
        "popq %%r14 \n"
        "popq %%r13 \n"
        "popq %%r12 \n"
        "popq %%r11 \n"
        "popq %%r10 \n"
        "popq %%r9 \n"
        "popq %%r8 \n"
        "popq %%rdi \n"
        "popq %%rsi \n"
        "popq %%rbp \n"
        "popq %%rcx \n"
        "popq %%rbx \n"
        "popfq \n"
        : [from_rsp] "=m" (from_thread->regs().rsp),
        [from_rip] "=m" (from_thread->regs().rip),
        [tss_rsp0l] "=m" (m_tss.rsp0l),
        [tss_rsp0h] "=m" (m_tss.rsp0h),
        "=d" (from_thread), 
        "=a" (to_thread) 
        : [to_rsp] "g" (to_thread->regs().rsp),
        [to_rsp0] "g" (to_thread->regs().rsp0),
        [to_rip] "c" (to_thread->regs().rip),
        [from_thread] "d" (from_thread),
        [to_thread] "a" (to_thread)
        : "memory", "rbx"
    );    

    dbgln_if(CONTEXT_SWITCH_DEBUG, "switch_context <-- from {} {} to {} {}", VirtualAddress(from_thread), *from_thread, VirtualAddress(to_thread), *to_thread);
}

UNMAP_AFTER_INIT void Processor::initialize_context_switching(Thread& initial_thread)
{
    VERIFY(initial_thread.process().is_kernel_process());

    auto& regs = initial_thread.regs();
    m_tss.iomapbase = sizeof(m_tss);
    m_tss.rsp0l = regs.rsp0 & 0xffffffff;
    m_tss.rsp0h = regs.rsp0 >> 32;

    m_scheduler_initialized = true;

    asm volatile(
        "movq %[new_rsp], %%rsp \n"
        "pushq %[from_to_thread] \n" 
        "pushq %[from_to_thread] \n" 
        "pushq %[new_rip] \n" 
        "cld \n"
        "pushq %[cpu] \n" 
        "call pre_init_finished \n"
        "pop %%rdi \n" 
        "call init_finished \n"
        "call post_init_finished \n"
        "movq 24(%%rsp), %%rdi \n" 
        "call enter_trap_no_irq \n"
        "retq \n"
        :: [new_rsp] "g" (regs.rsp),
        [new_rip] "a" (regs.rip),
        [from_to_thread] "b" (&initial_thread),
        [cpu] "c" ((u64)id())
    );

    VERIFY_NOT_REACHED();
}

}
