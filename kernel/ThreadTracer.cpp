/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/arch/x86/RegisterState.h>
#include <kernel/ThreadTracer.h>

namespace Kernel {

ThreadTracer::ThreadTracer(ProcessID tracer_pid)
    : m_tracer_pid(tracer_pid)
{
}

void ThreadTracer::set_regs(const RegisterState& regs)
{
    PtraceRegisters r {};
    copy_kernel_registers_into_ptrace_registers(r, regs);
    m_regs = r;
}

}
