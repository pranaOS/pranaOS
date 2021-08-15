/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/Optional.h>
#include <base/StdLibExtras.h>
#include <libjs/Runtime/VM.h>
#include <libweb/bindings/DOMExceptionWrapper.h>
#include <libweb/dom/ExceptionOr.h>

namespace Web::Bindings {

template<typename>
constexpr bool IsExceptionOr = false;

template<typename T>
constexpr bool IsExceptionOr<DOM::ExceptionOr<T>> = true;

template<typename T>
ALWAYS_INLINE bool throw_dom_exception(JS::VM& vm, JS::GlobalObject& global_object, DOM::ExceptionOr<T>& result)
{
    if (result.is_exception()) {
        result.materialized_exception(global_object)
            .visit(
                [&](NonnullRefPtr<DOM::DOMException> dom_exception) { vm.throw_exception(global_object, DOMExceptionWrapper::create(global_object, move(dom_exception))); },
                [&](auto* js_exception) { vm.throw_exception(global_object, js_exception); });
        return true;
    }
    return false;
}

namespace Detail {

template<typename T>
struct ExtractExceptionOrValueType {
    using Type = T;
};

template<typename T>
struct ExtractExceptionOrValueType<DOM::ExceptionOr<T>> {
    using Type = T;
};

template<>
struct ExtractExceptionOrValueType<void> {
    using Type = JS::Value;
};

template<>
struct ExtractExceptionOrValueType<DOM::ExceptionOr<Empty>> {
    using Type = JS::Value;
};

template<>
struct ExtractExceptionOrValueType<DOM::ExceptionOr<void>> {
    using Type = JS::Value;
};

}

template<typename T>
using ExtractExceptionOrValueType = typename Detail::ExtractExceptionOrValueType<T>::Type;

template<typename F, typename T = decltype(declval<F>()()), typename Ret = Conditional<!IsExceptionOr<T> && !IsVoid<T>, T, ExtractExceptionOrValueType<T>>>
Optional<Ret> throw_dom_exception_if_needed(auto&& vm, auto&& global_object, F&& fn)
{
    if constexpr (IsExceptionOr<T>) {
        auto&& result = fn();
        if (throw_dom_exception(vm, global_object, result))
            return {};
        if constexpr (requires(T v) { v.value(); })
            return result.value();
        else
            return JS::js_undefined();
    } else if constexpr (IsVoid<T>) {
        fn();
        return JS::js_undefined();
    } else {
        return fn();
    }
}

template<typename T>
bool should_return_empty(const Optional<T>& value)
{
    if constexpr (IsSame<JS::Value, T>)
        return !value.has_value() || value.value().is_empty();
    return !value.has_value();
}

}
