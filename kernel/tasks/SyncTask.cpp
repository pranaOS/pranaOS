/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/filesystem/VirtualFileSystem.h>
#include <kernel/Process.h>
#include <kernel/Sections.h>
#include <kernel/tasks/SyncTask.h>
#include <kernel/time/TimeManagement.h>

namespace Kernel {

UNMAP_AFTER_INIT void SyncTask::spawn()
{
    RefPtr<Thread> syncd_thread;
    Process::create_kernel_process(syncd_thread, "SyncTask", [] {
        dbgln("SyncTask is running");
        for (;;) {
            VirtualFileSystem::sync();
            (void)Thread::current()->sleep(Time::from_seconds(1));
        }
    });
}

}
