/*
 * Copyright (c) 2021, Krisna Pranav
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
        clear(false)
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

};

}
