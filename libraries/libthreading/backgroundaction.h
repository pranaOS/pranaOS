/**
 * @file backgroundaction.h
 * @author Krisna Pranav
 * @brief Background Action
 * @version 6.0
 * @date 2024-10-26
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/function.h>
#include <mods/nonnullrefptr.h>
#include <mods/optional.h>
#include <mods/queue.h>
#include <libcore/event.h>
#include <libcore/eventloop.h>
#include <libcore/object.h>
#include <libthreading/thread.h>

namespace Threading 
{
    /**
     * @tparam Result 
     */
    template<typename Result>
    class BackgroundAction;

    class BackgroundActionBase 
    {
        template<typename Result>
        friend class BackgroundAction;

    private:
        /**
         * @brief Construct a new Background Action Base object
         * 
         */
        BackgroundActionBase() { }

        static void enqueue_work(Function<void()>);

        static Thread& background_thread();
    };

    /**
     * @tparam Result 
     */
    template<typename Result>
    class BackgroundAction final : public Core::Object
        , private BackgroundActionBase {
        C_OBJECT(BackgroundAction);

    public:
        void cancel()
        {
            m_cancelled = true;
        }

        bool is_cancelled() const
        {
            return m_cancelled;
        }

        virtual ~BackgroundAction() { }

    private:
        /**
         * @brief Construct a new Background Action object
         * 
         * @param action 
         * @param on_complete 
         */
        BackgroundAction(Function<Result(BackgroundAction&)> action, Function<void(Result)> on_complete)
            : Core::Object(&background_thread())
            , m_action(move(action))
            , m_on_complete(move(on_complete))
        {
            enqueue_work([this, origin_event_loop = &Core::EventLoop::current()] {
                m_result = m_action(*this);
                if (m_on_complete) {
                    origin_event_loop->deferred_invoke([this] {
                        m_on_complete(m_result.release_value());
                        remove_from_parent();
                    });
                    origin_event_loop->wake();
                } else {
                    this->remove_from_parent();
                }
            });
        }

        bool m_cancelled { false };
        Function<Result(BackgroundAction&)> m_action;
        Function<void(Result)> m_on_complete;
        Optional<Result> m_result;
    }; // class BackgroundAction final : public Core::Object
} // namespace Threading
