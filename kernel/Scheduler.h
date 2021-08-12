/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Assertions.h>
#include <base/Function.h>
#include <base/IntrusiveList.h>
#include <base/Types.h>
#include <kernel/Forward.h>
#include <kernel/locking/SpinLock.h>
#include <kernel/time/TimeManagement.h>
#include <kernel/UnixTypes.h>

namespace Kernel {

struct RegisterState;

extern Thread* g_finalizer;
extern WaitQueue* g_finalizer_wait_queue;
extern Atomic<bool> g_finalizer_has_work;
extern RecursiveSpinLock g_scheduler_lock;

struct TotalTimeScheduled {
    u64 total { 0 };
    u64 total_kernel { 0 };
};

class Scheduler {
public:
    static void initialize();
    static Thread* create_ap_idle_thread(u32 cpu);
    static void set_idle_thread(Thread* idle_thread);
    static void timer_tick(const RegisterState&);
    [[noreturn]] static void start();
    static bool pick_next();
    static bool yield();
    static bool context_switch(Thread*);
    static void enter_current(Thread& prev_thread, bool is_first);
    static void leave_on_first_switch(u32 flags);
    static void prepare_after_exec();
    static void prepare_for_idle_loop();
    static Process* colonel();
    static void idle_loop(void*);
    static void invoke_async();
    static void notify_finalizer();
    static Thread& pull_next_runnable_thread();
    static Thread* peek_next_runnable_thread();
    static bool dequeue_runnable_thread(Thread&, bool = false);
    static void enqueue_runnable_thread(Thread&);
    static void dump_scheduler_state(bool = false);
    static bool is_initialized();
    static TotalTimeScheduled get_total_time_scheduled();
    static void add_time_scheduled(u64, bool);
    static u64 (*current_time)();
};

}
