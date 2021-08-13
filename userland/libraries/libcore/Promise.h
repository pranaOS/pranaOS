/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libcore/EventLoop.h>
#include <libcore/Object.h>

namespace Core {
template<typename Result>
class Promise : public Object {
    C_OBJECT(Promise);

private:
    Optional<Result> m_pending;

public:
    Function<void(Result&)> on_resolved;

    void resolve(Result&& result)
    {
        m_pending = move(result);
        if (on_resolved)
            on_resolved(m_pending.value());
    }

    bool is_resolved()
    {
        return m_pending.has_value();
    };

    Result await()
    {
        while (!is_resolved()) {
            Core::EventLoop::current().pump();
        }
        return m_pending.release_value();
    }

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
};
}
