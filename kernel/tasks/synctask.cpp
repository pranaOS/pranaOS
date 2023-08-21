/**
 * @file synctask.cpp
 * @author Krisna Pranav
 * @brief sync task
 * @version 6.0
 * @date 2023-08-21
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/process.h>
#include <kernel/filesystem/virtualfilesystem.h>
#include <kernel/tasks/synctask.h>
#include <kernel/time/timemanagement.h>

namespace Kernel
{   
    /// @breif: spawn
    void SyncTask::spwan()
    {
        RefPtr<Thread> syncd_thread;

        Proces::create_kernel_process(syncd_thread, "SyncTask", [] {
            dbg << "SynTask is running";
            VFS::the().sync();
            Thread::current()->sleep({1, 1});
        })
    }
} // namespace Kernel