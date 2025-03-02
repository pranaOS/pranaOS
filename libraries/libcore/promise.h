/**
 * @file promise.h
 * @author Krisna Pranav
 * @brief promise
 * @version 6.0
 * @date 2025-03-02
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <libcore/eventloop.h>
#include <libcore/object.h>

namespace Core 
{

    /**
     * @tparam Result 
     */
    template<typename Result>
    class Promise : public Object 
    {
        C_OBJECT(Promise);

    public:
        Function<void(Result&)> on_resolved;

        /**
         * @param result 
         */
        void resolve(Result&& result)
        {
            m_pending = move(result);
            if (on_resolved)
                on_resolved(m_pending.value());
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_resolved()
        {
            return m_pending.has_value();
        };

        /**
         * @return Result 
         */
        Result await()
        {
            while (!is_resolved()) {
                Core::EventLoop::current().pump();
            }

            return m_pending.release_value();
        }

        /**
         * @tparam T 
         * @param func 
         * @return RefPtr<Promise<T>> 
         */
        template<typename T>
        RefPtr<Promise<T>> map(T func(Result&))
        {
            RefPtr<Promise<T>> new_promise = Promise<T>::construct();
            on_resolved = [new_promise, func](Result& result) mutable {
                auto t = func(result);
                new_promise->resolve(move(t));
            };
            return new_promise;
        }

    private:
        Promise() = default;

        Optional<Result> m_pending;
    }; // class Promise : public Object

} // namespace Core
