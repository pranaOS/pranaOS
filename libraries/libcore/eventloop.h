/**
 * @file eventloop.h
 * @author Krisna Pranav
 * @brief Event Loop
 * @version 6.0
 * @date 2024-10-26
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/forward.h>
#include <mods/function.h>
#include <mods/hashmap.h>
#include <mods/noncopyable.h>
#include <mods/nonnullownptr.h>
#include <mods/nonnullrefptr.h>
#include <mods/time.h>
#include <mods/vector.h>
#include <mods/weakptr.h>
#include <libcore/deferredinvocationcontext.h>
#include <libcore/event.h>
#include <libcore/forward.h>
#include <libthreading/mutexprotected.h>
#include <sys/time.h>
#include <sys/types.h>

namespace Core 
{

    class EventLoop 
    {
    public:
        enum class MakeInspectable 
        {
            No,
            Yes,
        }; // enum class MakeInspectable 

        enum class ShouldWake 
        {
            No,
            Yes
        }; // enum class ShouldWake 

        /**
         * @brief Construct a new EventLoop object
         * 
         */
        explicit EventLoop(MakeInspectable = MakeInspectable::No);

        /**
         * @brief Destroy the EventLoop object
         * 
         */
        ~EventLoop();

        static void initialize_wake_pipes();

        int exec();

        enum class WaitMode 
        {
            WaitForEvents,
            PollForEvents,
        }; // enum class WaitMode 

        /**
         * @return size_t 
         */
        size_t pump(WaitMode = WaitMode::WaitForEvents);

        void spin_until(Function<bool()>);

        /**
         * @param receiver 
         */
        void post_event(Object& receiver, NonnullOwnPtr<Event>&&, ShouldWake = ShouldWake::No);

        /**
         * @return EventLoop& 
         */
        static EventLoop& current();

        /**
         * @return true 
         * @return false 
         */
        bool was_exit_requested() const 
        { 
            return m_exit_requested; 
        }

        /**
         * @param milliseconds 
         * @param should_reload 
         * @return int 
         */
        static int register_timer(Object&, int milliseconds, bool should_reload, TimerShouldFireWhenNotVisible);

        /**
         * @param timer_id 
         * @return true 
         * @return false 
         */
        static bool unregister_timer(int timer_id);

        static void register_notifier(Badge<Notifier>, Notifier&);
        static void unregister_notifier(Badge<Notifier>, Notifier&);

        void quit(int);
        void unquit();

        /**
         * @param other 
         */
        void take_pending_events_from(EventLoop& other)
        {
            m_queued_events.extend(move(other.m_queued_events));
        }

        static void wake_current();

        void wake();

        /**
         * @param signo 
         * @param handler 
         * @return int 
         */
        static int register_signal(int signo, Function<void(int)> handler);

        /**
         * @param handler_id 
         */
        static void unregister_signal(int handler_id);

        enum class ForkEvent 
        {
            Child,
        }; // enum class ForkEvent 
        
        static void notify_forked(ForkEvent);

        static bool has_been_instantiated();

        /**
         * @param invokee 
         */
        void deferred_invoke(Function<void()> invokee)
        {
            auto context = DeferredInvocationContext::construct();
            post_event(context, make<Core::DeferredInvocationEvent>(context, move(invokee)));
        }

    private:
        void wait_for_event(WaitMode);
        Optional<Time> get_next_timer_expiration();
        static void dispatch_signal(int);
        static void handle_signal(int);

        struct QueuedEvent 
        {
            MOD_MAKE_NONCOPYABLE(QueuedEvent);

        public:
            /**
             * @brief Construct a new QueuedEvent object
             * 
             * @param receiver 
             */
            QueuedEvent(Object& receiver, NonnullOwnPtr<Event>);

            /**
             * @brief Construct a new QueuedEvent object
             * 
             */
            QueuedEvent(QueuedEvent&&);

            /**
             * @brief Destroy the QueuedEvent object
             * 
             */
            ~QueuedEvent() = default;

            WeakPtr<Object> receiver;
            NonnullOwnPtr<Event> event;
        }; // struct QueuedEvent

        Vector<QueuedEvent, 64> m_queued_events;
        static pid_t s_pid;

        bool m_exit_requested { false };
        int m_exit_code { 0 };

        static thread_local int s_wake_pipe_fds[2];
        static thread_local bool s_wake_pipe_initialized;

        int (*m_wake_pipe_fds)[2];

        struct Private;
        NonnullOwnPtr<Private> m_private;
    }; // class EventLoop

    /**
     * @param invokee 
     */
    inline void deferred_invoke(Function<void()> invokee)
    {
        EventLoop::current().deferred_invoke(move(invokee));
    }

} // namespace Core
