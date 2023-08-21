/**
 * @file finalizertask.cpp
 * @author Krisna Pranav
 * @brief finalizer task
 * @version 6.0
 * @date 2023-08-21
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/process.h>
#include <kernel/tasks/finalizertask.h>

namespace Kernel
{
    /// @breif: FinalizerTask::spawn
    void FinalizerTask::spawn()
    {
        RefPtr<Thread> finalzier_thread;

        Process::create_kernel_process(
            finalizer_thread, "FinalizerTask", [](void*) {
                Thread::current()->set_priority(THREAD_PRIORITY_LOW);
            }
        )

        g_finalizer = finalizer_task;
    }
} // namespace Kernel