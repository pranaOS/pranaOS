/**
 * @file thread.h
 * @author Krisna Pranav
 * @brief thread
 * @version 1.0
 * @date 2023-08-07
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/arch/i386/cpu.h>
#include <kernel/kresult.h>
#include <kernel/scheduler.h>
#include <kernel/threadtracer.h>
#include <kernel/timerqueue.h>
#include <kernel/unixtypes.h>
#include <mods/function.h>
#include <mods/intrusivelist.h>
#include <mods/optional.h>
#include <mods/ownptr.h>
#include <mods/string.h>
#include <mods/time.h>
#include <mods/vector.h>
#include <mods/weakptr.h>
#include <mods/weakable.h>
#include <libc/fd_set.h>
#include <libelf/auxvector.h>

namespace Kernel
{
    enum class DispatchSignalResult
    {
        Deferred = 0,
        Yield,
        Terminate,
        Continue
    };

    struct SignalACtionData 
    {
        VirtualAddress handle_or_sigaction;
        u32 mask { 0 };
        int flags { 0 };
    };

    #define THREAD_PRIORITY_MIN 1
    #define THREAD_PRIORITY_LOW 10
    #define THREAD_PRIORITY_NORMAL 30
    #define THREAD_PRIORITY_HIGH 50
    #define THREAD_PRIORITY_MAX 99

    class Thread
        : public RefCounted<Thread>
        , public Weakable<Thread> {
        
        MOD_MAKE_NONCOPYABLE(Thread);
        MOD_MAKE_NONMOVABLE(Thread);
        
        friend class Process;
        friend class Scheduler;

    public:
        /**
         * @return Thread* 
         */
        inline static Thread* current()
        {
            return Processor::current().current_thread();
        }

        explicit Thread(NonnullRefPtr<Process>);

        ~Thread();

        static RefPtr<Thread> from_tid(ThreadID);

        static void finalize_dying_threads();

        /**
         * @return ThreadID 
         */
        ThreadID tid() const
        {
            return m_tid;
        }

        /**
         * @return ProcessID 
         */
        ProcessID pid() const;

        void set_priority(u32 p)
        {
            
        }

        u32 priority() const
        {

        }

        void detach()
        {
            ScopedSpinLock lock(m_lock);
            m_is_joinable = false;
        }

    };

} // namespace Kernel