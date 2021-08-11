/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/StdLibExtras.h>
#include <kernel/arch/x86/Processor.h>
#include <kernel/arch/x86/TrapFrame.h>
#include <kernel/Process.h>
#include <kernel/Random.h>
#include <kernel/Sections.h>
#include <kernel/Thread.h>

namespace Kernel {

#define ENTER_THREAD_CONTEXT_ARGS_SIZE (2 * 4) 

NAKED void thread_context_first_enter(void)
{
 
    asm(

        "    movl 8(%esp), %ebx \n" 
        "    cld \n"
        "    call context_first_init \n"
        "    addl $" __STRINGIFY(ENTER_THREAD_CONTEXT_ARGS_SIZE) ", %esp \n"
        "    movl %ebx, 0(%esp) \n" 
        "    jmp common_trap_exit \n"
    );
    
}

NAKED void do_assume_context(Thread*, u32)
{
    asm(
        "    movl 4(%esp), %ebx \n"
        "    movl 8(%esp), %esi \n"

        "    subl $(" __STRINGIFY(4 + REGISTER_STATE_SIZE + TRAP_FRAME_SIZE + 4) "), %esp \n"
        "    pushl %esi \n"
        "    pushl %ebx \n"
        "    cld \n"
        "    call do_init_context \n"
        "    addl $8, %esp \n"
        "    movl %eax, %esp \n" 
        "    pushl %ebx \n" 
        "    pushl %ebx \n" 
        "    pushl $thread_context_first_enter \n" 
        "    jmp enter_thread_context \n"
    );
}

String Processor::platform_string() const
{
    return "i386";
}

FlatPtr Processor::init_context(Thread& thread, bool leave_crit)
{
    VERIFY(is_kernel_mode());
    VERIFY(g_scheduler_lock.is_locked());
    if (leave_crit) {

        VERIFY(in_critical() == 2);
        m_in_critical = 1; 
    }

    u32 kernel_stack_top = thread.kernel_stack_top();

    kernel_stack_top -= round_up_to_power_of_two(get_fast_random<u8>(), 16);

    u32 stack_top = kernel_stack_top;

    VERIFY((cpu_flags() & 0x24000) == 0); 

    auto& regs = thread.regs();
    bool return_to_user = (regs.cs & 3) != 0;

    if (!return_to_user) {

        stack_top -= sizeof(RegisterState) - 2 * sizeof(u32);

        stack_top -= 2 * sizeof(u32);
        *reinterpret_cast<u32*>(kernel_stack_top - 2 * sizeof(u32)) = regs.esp;
        *reinterpret_cast<u32*>(kernel_stack_top - 3 * sizeof(u32)) = FlatPtr(&exit_kernel_thread);
    } else {
        stack_top -= sizeof(RegisterState);
    }

    stack_top -= sizeof(u32);
    *reinterpret_cast<u32*>(kernel_stack_top - sizeof(u32)) = 0;

    RegisterState& iretframe = *reinterpret_cast<RegisterState*>(stack_top);
    iretframe.ss = regs.ss;
    iretframe.gs = regs.gs;
    iretframe.fs = regs.fs;
    iretframe.es = regs.es;
    iretframe.ds = regs.ds;
    iretframe.edi = regs.edi;
    iretframe.esi = regs.esi;
    iretframe.ebp = regs.ebp;
    iretframe.esp = 0;
    iretframe.ebx = regs.ebx;
    iretframe.edx = regs.edx;
    iretframe.ecx = regs.ecx;
    iretframe.eax = regs.eax;
    iretframe.eflags = regs.eflags;
    iretframe.eip = regs.eip;
    iretframe.cs = regs.cs;
    if (return_to_user) {
        iretframe.userspace_esp = regs.esp;
        iretframe.userspace_ss = regs.ss;
    }

    stack_top -= sizeof(TrapFrame);
    TrapFrame& trap = *reinterpret_cast<TrapFrame*>(stack_top);
    trap.regs = &iretframe;
    trap.prev_irq_level = 0;
    trap.next_trap = nullptr;

    stack_top -= sizeof(u32); 
    *reinterpret_cast<u32*>(stack_top) = stack_top + 4;

    if constexpr (CONTEXT_SWITCH_DEBUG) {
        if (return_to_user) {
            dbgln("init_context {} ({}) set up to execute at eip={}:{}, esp={}, stack_top={}, user_top={}:{}",
                thread,
                VirtualAddress(&thread),
                iretframe.cs, regs.eip,
                VirtualAddress(regs.esp),
                VirtualAddress(stack_top),
                iretframe.userspace_ss,
                iretframe.userspace_esp);
        } else {
            dbgln("init_context {} ({}) set up to execute at eip={}:{}, esp={}, stack_top={}",
                thread,
                VirtualAddress(&thread),
                iretframe.cs, regs.eip,
                VirtualAddress(regs.esp),
                VirtualAddress(stack_top));
        }
    }

    regs.eip = FlatPtr(&thread_context_first_enter);
    regs.esp0 = kernel_stack_top;
    regs.esp = stack_top;
    regs.cs = GDT_SELECTOR_CODE0;
    regs.ds = GDT_SELECTOR_DATA0;
    regs.es = GDT_SELECTOR_DATA0;
    regs.gs = GDT_SELECTOR_DATA0;
    regs.ss = GDT_SELECTOR_DATA0;
    regs.gs = GDT_SELECTOR_PROC;
    return stack_top;
}

void Processor::switch_context(Thread*& from_thread, Thread*& to_thread)
{
    VERIFY(!in_irq());
    VERIFY(m_in_critical == 1);
    VERIFY(is_kernel_mode());

    dbgln_if(CONTEXT_SWITCH_DEBUG, "switch_context --> switching out of: {} {}", VirtualAddress(from_thread), *from_thread);

    from_thread->save_critical(m_in_critical);

    asm volatile(

        "pushfl \n"
        "pushl %%ebx \n"
        "pushl %%esi \n"
        "pushl %%edi \n"
        "pushl %%ebp \n"
        "movl %%esp, %[from_esp] \n"
        "movl $1f, %[from_eip] \n"
        "movl %[to_esp0], %%ebx \n"
        "movl %%ebx, %[tss_esp0] \n"
        "movl %[to_esp], %%esp \n"
        "pushl %[to_thread] \n"
        "pushl %[from_thread] \n"
        "pushl %[to_eip] \n"
        "cld \n"
        "jmp enter_thread_context \n"
        "1: \n"
        "popl %%edx \n"
        "popl %%eax \n"
        "popl %%ebp \n"
        "popl %%edi \n"
        "popl %%esi \n"
        "popl %%ebx \n"
        "popfl \n"
        : [from_esp] "=m" (from_thread->regs().esp),
          [from_eip] "=m" (from_thread->regs().eip),
          [tss_esp0] "=m" (m_tss.esp0),
          "=d" (from_thread), 
          "=a" (to_thread) 
        : [to_esp] "g" (to_thread->regs().esp),
          [to_esp0] "g" (to_thread->regs().esp0),
          [to_eip] "c" (to_thread->regs().eip),
          [from_thread] "d" (from_thread),
          [to_thread] "a" (to_thread)
        : "memory"
    );

    dbgln_if(CONTEXT_SWITCH_DEBUG, "switch_context <-- from {} {} to {} {}", VirtualAddress(from_thread), *from_thread, VirtualAddress(to_thread), *to_thread);
}

UNMAP_AFTER_INIT void Processor::initialize_context_switching(Thread& initial_thread)
{
    VERIFY(initial_thread.process().is_kernel_process());

    auto& regs = initial_thread.regs();
    m_tss.iomapbase = sizeof(m_tss);
    m_tss.esp0 = regs.esp0;
    m_tss.ss0 = GDT_SELECTOR_DATA0;

    m_scheduler_initialized = true;

    asm volatile(
        "movl %[new_esp], %%esp \n" 
        "pushl %[from_to_thread] \n" 
        "pushl %[from_to_thread] \n" 
        "pushl $" __STRINGIFY(GDT_SELECTOR_CODE0) " \n"
        "pushl %[new_eip] \n" 
        "movl %%esp, %%ebx \n"
        "addl $20, %%ebx \n" 
        "pushl %%ebx \n"
        "cld \n"
        "pushl %[cpu] \n"
        "call pre_init_finished \n"
        "call init_finished \n"
        "addl $4, %%esp \n"
        "call post_init_finished \n"
        "call enter_trap_no_irq \n"
        "addl $4, %%esp \n"
        "lret \n"
        :: [new_esp] "g" (regs.esp),
           [new_eip] "a" (regs.eip),
           [from_to_thread] "b" (&initial_thread),
           [cpu] "c" (id())
    );

    VERIFY_NOT_REACHED();
}
}
