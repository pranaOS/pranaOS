/**
 * @file waitqueue.cpp
 * @author Krisna Pranav
 * @brief wait queue 
 * @version 1.0
 * @date 2023-09-06
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/thread.h>
#include <kernel/waitqueue.h>
#include <kernel/debug.h>

namespace Kernel 
{

    /**
     * @param b 
     * @return true 
     * @return false 
     */
    bool WaitQueue::should_add_blocker(Thread::Blocker& b, void*)
    {
        VERIFY(m_lock.is_locked());
        VERIFY(b.blocker_type() == Thread::Blocker::Type::Queue);
        if (m_wake_requested) {
            m_wake_requested = false;
            dbgln_if(WAITQUEUE_DEBUG, "waitqueue {}: do not block the thread {}", this, b.thread());
            return false;
        }
        dbgln_if(WAITQUEUE_DEBUG, "waitqueue  {}: should block the thread {}", this, b.thread());
        return true;
    }

    /**
     * @return u32 
     */
    u32 WaitQueue::wake_one()
    {
        u32 did_wake = 0;
        SpinlockLocker lock(m_lock);
        dbgln_if(WAITQUEUE_DEBUG, "waitqueue {}: wake_one", this);
        bool did_unblock_one = unblock_all_blockers_whose_conditions_are_met_locked([&](Thread::Blocker& b, void*, bool& stop_iterating) {
            VERIFY(b.blocker_type() == Thread::Blocker::Type::Queue);
            auto& blocker = static_cast<Thread::WaitQueueBlocker&>(b);
            dbgln_if(WAITQUEUE_DEBUG, "waitqueue {}: wake_one unblocking {}", this, blocker.thread());

            if (blocker.unblock()) {
                stop_iterating = true;
                did_wake = 1;
                return true;
            }
            return false;
        });

        m_wake_requested = !did_unblock_one;
        dbgln_if(WAITQUEUE_DEBUG, "WaitQueue @ {}: wake_one woke {} threads", this, did_wake);
        return did_wake;
    }

    /**
     * @param wake_count 
     * @return u32 
     */
    u32 WaitQueue::wake_n(u32 wake_count)
    {
        if (wake_count == 0)
            return 0; 

        SpinlockLocker lock(m_lock);
        dbgln_if(WAITQUEUE_DEBUG, "waitqueue {}: wake_n({})", this, wake_count);
        u32 did_wake = 0;

        bool did_unblock_some = unblock_all_blockers_whose_conditions_are_met_locked([&](Thread::Blocker& b, void*, bool& stop_iterating) {
            VERIFY(b.blocker_type() == Thread::Blocker::Type::Queue);
            auto& blocker = static_cast<Thread::WaitQueueBlocker&>(b);
            dbgln_if(WAITQUEUE_DEBUG, "waitqueue {}: wake_n unblocking {}", this, blocker.thread());
            VERIFY(did_wake < wake_count);
            if (blocker.unblock()) {
                if (++did_wake >= wake_count)
                    stop_iterating = true;
                return true;
            }
            return false;
        });

        m_wake_requested = !did_unblock_some;
        dbgln_if(WAITQUEUE_DEBUG, "waitqueue {}: wake_n({}) woke {} threads", this, wake_count, did_wake);
        return did_wake;
    }

    /**
     * @return u32 
     */
    u32 WaitQueue::wake_all()
    {
        SpinlockLocker lock(m_lock);

        dbgln_if(WAITQUEUE_DEBUG, "waitqueue {}: wake_all", this);
        u32 did_wake = 0;

        bool did_unblock_any = unblock_all_blockers_whose_conditions_are_met_locked([&](Thread::Blocker& b, void*, bool&) {
            VERIFY(b.blocker_type() == Thread::Blocker::Type::Queue);
            auto& blocker = static_cast<Thread::WaitQueueBlocker&>(b);

            dbgln_if(WAITQUEUE_DEBUG, "waitqueue {}: wake_all unblocking {}", this, blocker.thread());

            if (blocker.unblock()) {
                did_wake++;
                return true;
            }
            return false;
        });
        m_wake_requested = !did_unblock_any;
        dbgln_if(WAITQUEUE_DEBUG, "waitqueue {}: wake_all woke {} threads", this, did_wake);
        return did_wake;
    }

} // namespace Kernel