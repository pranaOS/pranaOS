/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/Singleton.h>
#include <base/Time.h>
#include <kernel/Scheduler.h>
#include <kernel/Sections.h>
#include <kernel/time/TimeManagement.h>
#include <kernel/TimerQueue.h>

namespace Kernel {

static Singleton<TimerQueue> s_the;
static SpinLock<u8> g_timerqueue_lock;

Time Timer::remaining() const
{
    return m_remaining;
}

Time Timer::now(bool is_firing) const
{
    auto clock_id = m_clock_id;
    if (is_firing) {
        switch (clock_id) {
        case CLOCK_MONOTONIC:
            clock_id = CLOCK_MONOTONIC_COARSE;
            break;
        case CLOCK_MONOTONIC_RAW:

            break;
        case CLOCK_REALTIME:
            clock_id = CLOCK_REALTIME_COARSE;
            break;
        default:
            break;
        }
    }
    return TimeManagement::the().current_time(clock_id);
}

TimerQueue& TimerQueue::the()
{
    return *s_the;
}

UNMAP_AFTER_INIT TimerQueue::TimerQueue()
{
    m_ticks_per_second = TimeManagement::the().ticks_per_second();
}

bool TimerQueue::add_timer_without_id(NonnullRefPtr<Timer> timer, clockid_t clock_id, const Time& deadline, Function<void()>&& callback)
{
    if (deadline <= TimeManagement::the().current_time(clock_id))
        return false;

    timer->setup(clock_id, deadline, move(callback));

    ScopedSpinLock lock(g_timerqueue_lock);
    timer->m_id = 0; 
    add_timer_locked(move(timer));
    return true;
}

TimerId TimerQueue::add_timer(NonnullRefPtr<Timer>&& timer)
{
    ScopedSpinLock lock(g_timerqueue_lock);

    timer->m_id = ++m_timer_id_count;
    VERIFY(timer->m_id != 0); 
    auto id = timer->m_id;
    add_timer_locked(move(timer));
    return id;
}

void TimerQueue::add_timer_locked(NonnullRefPtr<Timer> timer)
{
    Time timer_expiration = timer->m_expires;

    timer->clear_cancelled();
    timer->clear_callback_finished();
    timer->set_in_use();

    auto& queue = queue_for_timer(*timer);
    if (queue.list.is_empty()) {
        queue.list.append(timer.leak_ref());
        queue.next_timer_due = timer_expiration;
    } else {
        Timer* following_timer = nullptr;
        for (auto& t : queue.list) {
            if (t.m_expires > timer_expiration) {
                following_timer = &t;
                break;
            }
        }
        if (following_timer) {
            bool next_timer_needs_update = queue.list.first() == following_timer;
            queue.list.insert_before(*following_timer, timer.leak_ref());
            if (next_timer_needs_update)
                queue.next_timer_due = timer_expiration;
        } else {
            queue.list.append(timer.leak_ref());
        }
    }
}

TimerId TimerQueue::add_timer(clockid_t clock_id, const Time& deadline, Function<void()>&& callback)
{
    auto expires = TimeManagement::the().current_time(clock_id);
    expires = expires + deadline;
    auto timer = new Timer();
    VERIFY(timer);
    timer->setup(clock_id, expires, move(callback));
    return add_timer(adopt_ref(*timer));
}

bool TimerQueue::cancel_timer(TimerId id)
{
    Timer* found_timer = nullptr;
    Queue* timer_queue = nullptr;

    ScopedSpinLock lock(g_timerqueue_lock);
    for (auto& timer : m_timer_queue_monotonic.list) {
        if (timer.m_id == id) {
            found_timer = &timer;
            timer_queue = &m_timer_queue_monotonic;
            break;
        }
    }

    if (found_timer == nullptr) {
        for (auto& timer : m_timer_queue_realtime.list) {
            if (timer.m_id == id) {
                found_timer = &timer;
                timer_queue = &m_timer_queue_realtime;
                break;
            }
        };
    }

    if (found_timer) {
        VERIFY(timer_queue);
        remove_timer_locked(*timer_queue, *found_timer);
        return true;
    }

    for (auto& timer : m_timers_executing) {
        if (timer.m_id == id) {
            found_timer = &timer;
            break;
        }
    }

    if (!found_timer)
        return false;

    NonnullRefPtr<Timer> executing_timer(*found_timer);
    lock.unlock();

    if (!found_timer->set_cancelled()) {

        lock.lock();
        VERIFY(m_timers_executing.contains(*found_timer));
        m_timers_executing.remove(*found_timer);
        return true;
    }

    while (!found_timer->is_callback_finished())
        Processor::wait_check();

    return true;
}

bool TimerQueue::cancel_timer(Timer& timer, bool* was_in_use)
{
    bool in_use = timer.is_in_use();
    if (was_in_use)
        *was_in_use = in_use;

    if (!in_use) {
        VERIFY(!timer.m_list_node.is_in_list());
        return false;
    }

    bool did_already_run = timer.set_cancelled();
    auto& timer_queue = queue_for_timer(timer);
    if (!did_already_run) {
        timer.clear_in_use();

        ScopedSpinLock lock(g_timerqueue_lock);
        if (timer_queue.list.contains(timer)) {

            VERIFY(timer.ref_count() > 1);
            remove_timer_locked(timer_queue, timer);
            return true;
        }

        VERIFY(m_timers_executing.contains(timer));
        m_timers_executing.remove(timer);
        return true;
    }

    while (!timer.is_callback_finished())
        Processor::wait_check();

    return false;
}

void TimerQueue::remove_timer_locked(Queue& queue, Timer& timer)
{
    bool was_next_timer = (queue.list.first() == &timer);
    queue.list.remove(timer);
    auto now = timer.now(false);
    if (timer.m_expires > now)
        timer.m_remaining = timer.m_expires - now;

    if (was_next_timer)
        update_next_timer_due(queue);
    
    timer.unref();
}

void TimerQueue::fire()
{
    ScopedSpinLock lock(g_timerqueue_lock);

    auto fire_timers = [&](Queue& queue) {
        auto* timer = queue.list.first();
        VERIFY(timer);
        VERIFY(queue.next_timer_due == timer->m_expires);

        while (timer && timer->now(true) > timer->m_expires) {
            queue.list.remove(*timer);

            m_timers_executing.append(*timer);

            update_next_timer_due(queue);

            lock.unlock();

            // Defer executing the timer outside of the irq handler
            Processor::deferred_call_queue([this, timer]() {
                // Check if we were cancelled in between being triggered
                // by the timer irq handler and now. If so, just drop
                // our reference and don't execute the callback.
                if (!timer->set_cancelled()) {
                    timer->m_callback();
                    ScopedSpinLock lock(g_timerqueue_lock);
                    m_timers_executing.remove(*timer);
                }
                timer->clear_in_use();
                timer->set_callback_finished();
                // Drop the reference we added when queueing the timer
                timer->unref();
            });

            lock.lock();
            timer = queue.list.first();
        }
    };

    if (!m_timer_queue_monotonic.list.is_empty())
        fire_timers(m_timer_queue_monotonic);
    if (!m_timer_queue_realtime.list.is_empty())
        fire_timers(m_timer_queue_realtime);
}

void TimerQueue::update_next_timer_due(Queue& queue)
{
    VERIFY(g_timerqueue_lock.is_locked());

    if (auto* next_timer = queue.list.first())
        queue.next_timer_due = next_timer->m_expires;
    else
        queue.next_timer_due = {};
}

}
