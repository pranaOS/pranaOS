/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/locking/SpinLock.h>
#include <kernel/Process.h>
#include <kernel/Sections.h>
#include <kernel/WaitQueue.h>
#include <kernel/WorkQueue.h>

namespace Kernel {

WorkQueue* g_io_work;

UNMAP_AFTER_INIT void WorkQueue::initialize()
{
    g_io_work = new WorkQueue("IO WorkQueue");
}

UNMAP_AFTER_INIT WorkQueue::WorkQueue(const char* name)
{
    RefPtr<Thread> thread;
    Process::create_kernel_process(thread, name, [this] {
        for (;;) {
            WorkItem* item;
            bool have_more;
            {
                ScopedSpinLock lock(m_lock);
                item = m_items.take_first();
                have_more = !m_items.is_empty();
            }
            if (item) {
                item->function();
                delete item;

                if (have_more)
                    continue;
            }
            [[maybe_unused]] auto result = m_wait_queue.wait_on({});
        }
    });

    m_thread = thread.release_nonnull();
}

void WorkQueue::do_queue(WorkItem* item)
{
    {
        ScopedSpinLock lock(m_lock);
        m_items.append(*item);
    }
    m_wait_queue.wake_one();
}

}
