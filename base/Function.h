/*
 * Copyright (c) 2021, Krisna Pranav
 * Copyright (C) 2016 Apple Inc. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <base/Assertions.h>
#include <base/Atomic.h>
#include <base/BitCast.h>
#include <base/Noncopyable.h>
#include <base/ScopeGuard.h>
#include <base/StdLibExtras.h>
#include <base/Types.h>

namespace Base {

template<typename>
class Function;

template<typename F>
inline constexpr bool IsFunctionPointer = (IsPointer<F> && IsFunction<RemovePointer<F>>);

template<typename F>
inline constexpr bool IsFunctionObject = (!IsFunctionPointer<F> && IsRvalueReference<F&&>);

template<typename Out, typename... In>
class Function<Out(In...)>{
    BASE_MAKE_NONCOPYABLE(Function);

public:
    Function() = default;
    Function(std::nullptr_t)
    {
    }

    ~Function()
    {
        clear(false);
    }

    template<typename CallableType>
    Function(CallableType&& callable) requires((IsFunctionObject<CallableType> && IsCallableWithArguments<CallableType, In...> && !IsSame<RemoveCVReference<CallableType>, Function>))
    {
        init_with_callable(forward<CallableType>(callable));
    }

    template<typename FunctionType>
    Function(FunctionType f) requires((IsFunctionPointer<FunctionType> && IsCallableWithArguments<RemovePointer<FunctionType>, In...> && !IsSame<RemoveCVReference<FunctionType>, Function>))
    {
        init_with_callable(move(f));
    }

    Function(Function&& other)
    {
        move_from(move(other));
    }

    // Note: Despite this method being const, a mutable lambda _may_ modify its own captures.
    Out operator()(In... in) const
    {
        auto* wrapper = callable_wrapper();
        VERIFY(wrapper);
        ++m_call_nesting_level;
        ScopeGuard guard([this] {
            if (--m_call_nesting_level == 0 && m_deferred_clear)
                const_cast<Function*>(this)->clear(false);
        });
        return wrapper->call(forward<In>(in)...);
    }

    explicit operator bool() const { return !!callable_wrapper(); }

    template<typename CallableType>
    Function& operator=(CallableType&& callable) requires((IsFunctionObject<CallableType> && IsCallableWithArguments<CallableType, In...>))
    {
        clear();
        init_with_callable(forward<CallableType>(callable));
        return *this;
    }

    template<typename FunctionType>
    Function& operator=(FunctionType f) requires((IsFunctionPointer<FunctionType> && IsCallableWithArguments<RemovePointer<FunctionType>, In...>))
    {
        clear();
        if (f)
            init_with_callable(move(f));
        return *this;
    }

    Function& operator=(std::nullptr_t)
    {
        clear();
        return *this;
    }

    Function& operator=(Function&& other)
    {
        if (this != &other) {
            clear();
            move_from(move(other));
        }
        return *this;
    }
};

}
