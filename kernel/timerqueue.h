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

    public:
        /**
         * @param clock_id 
         * @param expires 
         * @param callback 
         */
        Timer(clockid_t clock_id, u64 expires, Function<void()>&& callback)
            : m_clock_id(clock_id)
            , m_expires(expires)
            , m_callback(move(callback))
        {}

        /// @breif: destroy
        ~Timer()
        {
            ASSERT(!is_queued());
        }

        timespec remaining() const;
    
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

    class TimerQueue
    {
    
    private:
        struct Queue
        {
            InlineLinkedList<Timer> list;
            u64 next_timer_due { 0 };
        };

        void remove_timer_locked(Queue&, Timer&);

        void update_next_timer_due(Queue&);

        void add_timer_locked(NonnullRefPtr<Timer>);

        /**
         * @param timer 
         * @return Queue& 
         */
        Queue& queue_for_timer(Timer& timer)
        {
            switch (timer.m_clock_id) {
            case CLOCK_MONOTONIC:
                return m_timer_queue_monotonic;
            case CLOCK_REALTIME:
                return m_timer_queue_realtime;
            default:
                ASSERT_NOT_REACHED();
            }
        }

        /**
         * @param ticks 
         * @return timespec 
         */
        timespec ticks_to_time(clockid_t, u64 ticks) const;

        /**
         * @return u64 
         */
        u64 time_to_ticks(clockid_t, const timespec&) const;

        u64 m_timer_id_count { 0 };
        u64 m_ticks_per_second { 0 };
        Queue m_timer_queue_monotonic;
        Queue m_timer_queue_realtime;
        InlineLinkedList<Timer> m_timers_executing;

    }; // class TimerQueue

} // namespace Kernel