/**
 * @file ptrace.h
 * @author Krisna Pranav
 * @brief ptrace
 * @version 6.0
 * @date 2023-09-06
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
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
}