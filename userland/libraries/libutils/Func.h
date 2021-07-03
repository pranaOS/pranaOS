/*
 * Copyright (c) 2021, evilbat831
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <assert.h>
#include <libutils/OwnPtr.h>
#include <libutils/Std.h>
#include <libutils/Traits.h>

namespace Utils
{

template <typename>
struct Func;

template <typename Out, typename... In>
struct Func<Out(In...)>
{
private:
    struct CallableWrapperBase
    {
        virtual ~CallableWrapperBase() {}
        virtual Out call(In...) const = 0;
    };

    template <typename TCallable>
    struct CallableWrapper final : public CallableWrapperBase
    {
        TCallable callable;

        NONCOPYABLE(CallableWrapper);

        explicit CallableWrapper(TCallable &&callable)
            : callable{std::move(callable)}
        {
        }

        Out call(In... in) const final override
        {
            return callable(std::forward<In>(in)...);
        }
    };

    OwnPtr<CallableWrapperBase> _wrapper;
    
}
