/**
 * @file scheduler.h
 * @author Krisna Pranav
 * @brief scheduler
 * @version 6.0
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
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
    struct RegisterState;
    struct SchedulerData;

    // externs the struct and class.
    extern Thread* g_finalizer;
    extern WaitQueue* g_finalizer_wait_queue;
    extern Atomic<bool> g_finalizer_has_work;
    extern SchedulerData* g_scheduler_data;
    extern RecursiveSpinLock g_scheduler_lock;

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
        static bool pick_next();
        static bool yield();
        static void yield_from_critical();

        /**
         * @param reason 
         * @return true 
         * @return false 
         */
        static bool donate_to_and_switch(Thread*, const char* reason);

        /**
         * @param reason 
         * @return true 
         * @return false 
         */
        static bool donate_to(RefPtr<Thread>&, const char* reason);

        /**
         * @return true 
         * @return false 
         */
        static bool context_switch(Thread*);

        /**
         * @param prev_thread 
         * @param is_first 
         */
        static void enter_current(Thread& prev_thread, bool is_first);

        /**
         * @param flags 
         */
        static void leave_on_first_switch(u32 flags);

        static void prepare_after_exec();

        static void prepare_for_idle_loop();

        /**
         * @return Process* 
         */
        static Process* colonel();

        static void beep();
        static void idle_loop(void*);
        static void invoke_async();
        static void notify_finalizer();

        /**
         * @tparam Callback 
         * @return IterDecision 
         */
        template<typename Callback>
        static inline IterDecision for_each_runnable(Callback);
        
        /**
         * @tparam Callback 
         * @return IterDecision 
         */
        template<typename Callback>
        static inline IterDecision for_each_nonrunnable(Callback);

        /**
         * @param thread 
         */
        static void init_thread(Thread& thread);
    }; // class Scheduler

}
