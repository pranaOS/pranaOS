/**
 * @file timerqueue.h
 * @author Krisna Pranav
 * @brief timerqueue
 * @version 6.0
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
        {
        }

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

        /**
         * @return u64 
         */
        u64 now() const;
    }; // class Timer

    class TimerQueue 
    {
        friend class Timer;

    public:
        TimerQueue();
        static TimerQueue& the();

        /**
         * @return TimerId 
         */
        TimerId add_timer(NonnullRefPtr<Timer>&&);

        /**
         * @return RefPtr<Timer> 
         */
        RefPtr<Timer> add_timer_without_id(clockid_t, const timespec&, Function<void()>&&);

        /**
         * @param timeout 
         * @param callback 
         * @return TimerId 
         */
        TimerId add_timer(clockid_t, timeval& timeout, Function<void()>&& callback);

        /**
         * @param id 
         * @return true 
         * @return false 
         */
        bool cancel_timer(TimerId id);

        /**
         * @return true 
         * @return false 
         */
        bool cancel_timer(Timer&);

        /**
         * @param timer 
         * @return true 
         * @return false 
         */
        bool cancel_timer(NonnullRefPtr<Timer>&& timer)
        {
            return cancel_timer(*move(timer));
        }

        /// @brief: fire.
        void fire();

    private:
        struct Queue 
        {
            InlineLinkedList<Timer> list;
            u64 next_timer_due { 0 };
        }; // struct Queue

        /// @breif: remove_timer_locked
        void remove_timer_locked(Queue&, Timer&);

        /// @breif: update_next_timer_due
        /// @param Queue
        void update_next_timer_due(Queue&);

        /// @brief: add_timer_locked
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

} // namespaace Kernel
