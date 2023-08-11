/**
 * @file lock.h
 * @author Krisna Pranav
 * @brief lock
 * @version 1.0
 * @date 2023-08-11
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/arch/i386/cpu.h>
#include <kernel/forward.h>
#include <kernel/thread.h>
#include <kernel/waitqueue.h>
#include <mods/atomic.h>
#include <mods/types.h>
#include <mods/assertions.h>

namespace Kernel
{
    class Lock
    {
    public:
        Lock(const char* name = nullptr)
            : m_name(name)
        {}

        ~Lock() {}

    private:
        Atomic<bool> m_lock { false };
        const char* m_name { nullptr };

        WaitQueue m_queue;

        Mode m_mode { Mode::Unlocked };

        u32 m_times_locked { 0 };

        RefPtr<Thread> m_holder;
    }; // class Lock
} // namespace Kernel