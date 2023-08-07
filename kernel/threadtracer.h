/**
 * @file threadtracer.h
 * @author Krisna Pranav
 * @brief threadtracer
 * @version 1.0
 * @date 2023-08-07
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/nonnullownptr.h>
#include <mods/optional.h>
#include <kernel/unixtypes.h>
#include <libc/sys/arch/i386/regs.h>

namespace Kernel
{
    class ThreadTracer
    {
    public:

        /**
         * @param tracer 
         * @return NonnullOwnPtr<ThreadTracer> 
         */
        static NonnullOwnPtr<ThreadTracer> create(ProcessID tracer)
        {
            return make<ThreadTracer>(tracer);
        }

        /**
         * @return ProcessID 
         */
        ProcessID tracer_pid() const
        {
            return m_tracer_pid;
        }

        bool has_pending_signal(u32 signal) const
        {
            return m_pending_signals & (1 << (signal));
        }

        /**
         * @param signal 
         */
        void set_signal(u32 signal)
        {
            m_pending_signals |= (1 << (signal - 1));
        }

        /**
         * @param signal 
         */
        void unset_signal(u32 signal)
        {
            m_pending_signals &= (1 << (signal - 1));
        }

        /**
         * @return const PtraceRegister& 
         */
        const PtraceRegister& regs() const
        {
            ASSERT(m_regs.has_value());
            return m_regs.value();
        }

        explicit ThreadTracer(ProcessID);

    private:
        ProcessID m_tracer_id { -1 };

        u32 m_pending_signals { 0 };

        bool m_trace_syscalls { false };

        Optional<PtraceRegisters> m_regs; 

    }; // class ThreadTracer
} // namespace Kernel
