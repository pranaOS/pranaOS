/**
 * @file ptrace.h
 * @author Krisna Pranav
 * @brief ptrace
 * @version 6.0
 * @date 2023-09-06
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/kresult.h>
#include <kernel/process.h>
#include <kernel/api/syscall.h>
#include <libc/sys/arch/i386/regs.h>

namespace Ptrace 
{

    /**
     * @param params 
     * @param caller 
     * @return KResultOr<u32> 
     */
    KResultOr<u32> handle_syscall(const Kernel::Syscall::SC_ptrace_params& params, Process& caller);

    void copy_kernel_registers_into_ptrace_registers(PtraceRegisters&, const RegisterState&);
    void copy_ptrace_registers_into_kernel_registers(RegisterState&, const PtraceRegisters&);

} // namespace Kernel