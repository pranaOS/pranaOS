/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Optional.h>
#include <base/OwnPtr.h>
#include <kernel/UnixTypes.h>
#include <libc/sys/arch/i386/regs.h>

namespace Kernel {

class ThreadTracer {
public:
    static OwnPtr<ThreadTracer> create(ProcessID tracer) { return try_make<ThreadTracer>(tracer); }

    ProcessID tracer_pid() const { return m_tracer_pid; }
    bool has_pending_signal(u32 signal) const { return m_pending_signals & (1 << (signal - 1)); }
    void set_signal(u32 signal) { m_pending_signals |= (1 << (signal - 1)); }
    void unset_signal(u32 signal) { m_pending_signals &= ~(1 << (signal - 1)); }

    bool is_tracing_syscalls() const { return m_trace_syscalls; }
    void set_trace_syscalls(bool val) { m_trace_syscalls = val; }

    void set_regs(const RegisterState& regs);
    void set_regs(const PtraceRegisters& regs) { m_regs = regs; }
    bool has_regs() const { return m_regs.has_value(); }
    const PtraceRegisters& regs() const
    {
        VERIFY(m_regs.has_value());
        return m_regs.value();
    }

    explicit ThreadTracer(ProcessID);

private:
    ProcessID m_tracer_pid { -1 };

    u32 m_pending_signals { 0 };

    bool m_trace_syscalls { false };
    Optional<PtraceRegisters> m_regs;
};

}
