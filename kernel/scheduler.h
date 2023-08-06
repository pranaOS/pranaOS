/**
 * @file scheduler.h
 * @author Krisna Pranav
 * @brief scheduler
 * @version 1.0
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/spinlock.h>
#include <kernel/unixtypes.h>
#include <kernel/time/timemanagement.h>
#include <mods/assertions.h>
#include <mods/function.h>
#include <mods/intrusivelist.h>
#include <mods/types.h>

namespace Kernel
{
    class Process;
    class Thread;
    class WaitQueue;
    class RegisterState;
    
    extern Thread* g_finalizer;
    extern WaitQueue* g_finalizer_wait_queue;
    extern Atomic<bool> g_finalizer_has_work;

    class Scheduler
    {
    public:
        static void initialize();

        /**
         * @param cpu 
         * @return Thread* 
         */
        static Thread* create_ap_idle_thread(u32 cpu);

        /**
         * @param idle_thread 
         */
        static void set_idle_thread(Thread* idle_thread);

        static void timer_tick(const RegisterState&);

        [[noreturn]] static void start();

        /**
         * @param reason 
         * @return true 
         * @return false 
         */
        static bool donate_to_and_switch(Thread*, const char* reason);

        template<typename Callback>
        static inline IterationDecision for_each_runnable(Callable);

        template<typename Callback>
        static inline IterationDecision for_each_nonrunnable(Callback);

        /**
         * @param thread 
         */
        static void init_thread(Thread& thread);

    }; // class Scheduler
} // namespace Kernel