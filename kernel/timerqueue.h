/**
 * @file timerqueue.h
 * @author Krisna Pranav
 * @brief timerqueue
 * @version 1.0
 * @date 2023-08-07
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/time/timemanagement.h>
#include <mods/function.h>
#include <mods/inlinelinkedlist.h>
#include <mods/nonnullrefptr.h>
#include <mods/refcounted.h>

namespace Kernel
{
    typedef u64 TimerId;

    class Timer : public RefCounted<Timer> 
        , public InlineLinkedListNode<Timer> {
        
        friend class TimerQueue;
        friend class InlineLinkedListNode<Timer>;
    
    private:
        TimerId m_id;
        clockid_t m_clock_id;
        u64 m_expires;
        u64 m_remaining { 0 };
        Function<void()> m_callback;
        Timer* m_next { nullptr };
        Timer* m_prev { nullptr };
        Atomic<bool> m_queued { false };

        /**
         * @param rhs 
         * @return true 
         * @return false 
         */
        bool operator<(const Timer& rhs) const
        {
            return m_expires < rhs.m_expires;
        }

        /**
         * @param rhs 
         * @return true 
         * @return false 
         */
        bool operator>(const Timer& rhs) const
        {
            return m_expires > rhs.m_expires;
        }

        /**
         * @param rhs 
         * @return true 
         * @return false 
         */
        bool operator==(const Timer& rhs) const
        {
            return m_id == rhs.m_id;
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_queued() const 
        { 
            return m_queued.load(Mods::MemoryOrder::memory_order_relaxed); 
        }

        /**
         * @param queued 
         */
        void set_queued(bool queued) 
        { 
            m_queued.store(queued, Mods::MemoryOrder::memory_order_relaxed); 
        }

    }; // class Timer
} // namespace Kernel