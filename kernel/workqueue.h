/**
 * @file workqueue.h
 * @author Krisna Pranav
 * @brief Work Queue
 * @version 6.0
 * @date 2024-09-30
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/intrusivelist.h>
#include <kernel/forward.h>
#include <kernel/locking/spinlockprotected.h>
#include <kernel/waitqueue.h>

namespace Kernel 
{
    extern WorkQueue* g_io_work;

    class WorkQueue 
    {
        MOD_MAKE_NONCOPYABLE(WorkQueue);
        MOD_MAKE_NONMOVABLE(WorkQueue);

    public:
        static void initialize();

        /**
         * @param function 
         * @param data 
         * @param free_data 
         */
        void queue(void (*function)(void*), void* data = nullptr, void (*free_data)(void*) = nullptr)
        {
            auto* item = new WorkItem; 
            item->function = [function, data, free_data] {
                function(data);
                if (free_data)
                    free_data(data);
            };
            do_queue(item);
        }

        /**
         * @tparam Function 
         * @param function 
         */
        template<typename Function>
        void queue(Function function)
        {
            auto* item = new WorkItem; 
            item->function = Function(function);
            do_queue(item);
        }

    private:
        explicit WorkQueue(StringView);

        struct WorkItem 
        {
        public:
            IntrusiveListNode<WorkItem> m_node;
            Function<void()> function;
        }; // struct WorkItem

        void do_queue(WorkItem*);

        RefPtr<Thread> m_thread;
        WaitQueue m_wait_queue;
        SpinlockProtected<IntrusiveList<&WorkItem::m_node>> m_items;
    }; // class WorkQueue
} // namespace Kernel