/**
 * @file waitqueue.cpp
 * @author Krisna Pranav
 * @brief wait queue 
 * @version 1.0
 * @date 2023-09-06
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/thread.h>
#include <kernel/waitqueue.h>

namespace Kernel 
{

    /**
     * @param b 
     * @param data 
     * @return true 
     * @return false 
     */
    bool WaitQueue::should_add_blocker(Thread::Blocker& b, void* data)
    {
        ASSERT(data != nullptr); 
        ASSERT(m_lock.is_locked());
        ASSERT(b.blocker_type() == Thread::Blocker::Type::Queue);
        if (m_wake_requested) {
            m_wake_requested = false;
    #ifdef WAITQUEUE_DEBUG
            dbg() << "WaitQueue @ " << this << ": do not block thread " << *static_cast<Thread*>(data) << ", wake was pending";
    #endif
            return false;
        }
    #ifdef WAITQUEUE_DEBUG
        dbg() << "WaitQueue @ " << this << ": should block thread " << *static_cast<Thread*>(data);
    #endif
        return true;
    }

    /**
     * @brief wake one 
     * 
     */
    void WaitQueue::wake_one()
    {
        ScopedSpinLock lock(m_lock);

    #ifdef WAITQUEUE_DEBUG
        dbg() << "WaitQueue @ " << this << ": wake_one";
    #endif
        bool did_unblock_one = do_unblock_some([&](Thread::Blocker& b, void* data, bool& stop_iterating) {
            ASSERT(data);
            ASSERT(b.blocker_type() == Thread::Blocker::Type::Queue);
            auto& blocker = static_cast<Thread::QueueBlocker&>(b);
    #ifdef WAITQUEUE_DEBUG
            dbg() << "WaitQueue @ " << this << ": wake_one unblocking " << *static_cast<Thread*>(data);
    #endif
            if (blocker.unblock()) {
                stop_iterating = true;
                return true;
            }
            return false;
        });
        m_wake_requested = !did_unblock_one;
    }

    /**
     * @param wake_count 
     */
    void WaitQueue::wake_n(u32 wake_count)
    {
        if (wake_count == 0)
            return; 

        ScopedSpinLock lock(m_lock);

    #ifdef WAITQUEUE_DEBUG
        dbg() << "WaitQueue @ " << this << ": wake_n(" << wake_count << ")";
    #endif

        bool did_unblock_some = do_unblock_some([&](Thread::Blocker& b, void* data, bool& stop_iterating) {
            ASSERT(data);
            ASSERT(b.blocker_type() == Thread::Blocker::Type::Queue);
            auto& blocker = static_cast<Thread::QueueBlocker&>(b);

    #ifdef WAITQUEUE_DEBUG
            dbg() << "WaitQueue @ " << this << ": wake_n unblocking " << *static_cast<Thread*>(data);
    #endif

            ASSERT(wake_count > 0);

            if (blocker.unblock()) {
                if (--wake_count == 0)
                    stop_iterating = true;
                return true;
            }
            return false;
        });

        m_wake_requested = !did_unblock_some;
    }

    void WaitQueue::wake_all()
    {
        ScopedSpinLock lock(m_lock);
    #ifdef WAITQUEUE_DEBUG
        dbg() << "WaitQueue @ " << this << ": wake_all";
    #endif
        bool did_unblock_any = do_unblock_all([&](Thread::Blocker& b, void* data) {
            ASSERT(data);
            ASSERT(b.blocker_type() == Thread::Blocker::Type::Queue);
            auto& blocker = static_cast<Thread::QueueBlocker&>(b);
    #ifdef WAITQUEUE_DEBUG
            dbg() << "WaitQueue @ " << this << ": wake_all unblocking " << *static_cast<Thread*>(data);
    #endif
            return blocker.unblock();
        });
        m_wake_requested = !did_unblock_any;
    }

} // namespace Kernel