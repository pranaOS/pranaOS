/**
 * @file threadtracer.cpp
 * @author Krisna Pranav
 * @brief Thread Tracer
 * @version 6.0
 * @date 2024-10-11
 * 
 * @copyright Copyright (c) 2021-2024, pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/arch/registerstate.h>
#include <kernel/threadtracer.h>

namespace Kernel 
{

    /**
     * @brief Construct a new ThreadTracer::ThreadTracer object
     * 
     * @param tracer_pid 
     */
    ThreadTracer::ThreadTracer(ProcessID tracer_pid)
        : m_tracer_pid(tracer_pid)
    {
    }

    /**
     * @param regs 
     */
    void ThreadTracer::set_regs(const RegisterState& regs)
    {
        PtraceRegisters r {};
        copy_kernel_registers_into_ptrace_registers(r, regs);
        m_regs = r;
    }

} // namespace Kernel