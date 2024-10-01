/**
 * @file workqueue.cpp
 * @author Krisna Pranav
 * @brief Work Queue
 * @version 6.0
 * @date 2024-10-01
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/process.h>
#include <kernel/sections.h>
#include <kernel/waitqueue.h>
#include <kernel/workqueue.h>

namespace Kernel 
{

    WorkQueue* g_io_work;

    /**
     * @return UNMAP_AFTER_INIT 
     */
    UNMAP_AFTER_INIT void WorkQueue::initialize()
    {
        g_io_work = new WorkQueue("IO WorkQueue");
    }

    /**
     * @param name 
     * @return UNMAP_AFTER_INIT 
     */
    UNMAP_AFTER_INIT WorkQueue::WorkQueue(StringView name)
    {
        RefPtr<Thread> thread;
        auto name_kstring = KString::try_create(name);
        if (name_kstring.is_error())
            TODO();
            
        (void)Process::create_kernel_process(thread, name_kstring.release_value(), [this] {
            for (;;) {
                WorkItem* item;
                bool have_more;

                m_items.with([&](auto& items) {
                    item = items.take_first();
                    have_more = !items.is_empty();
                });

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

    /**
     * @param item 
     */
    void WorkQueue::do_queue(WorkItem* item)
    {
        m_items.with([&](auto& items) {
            items.append(*item);
        });
        m_wait_queue.wake_one();
    }

} // namespace Kernel