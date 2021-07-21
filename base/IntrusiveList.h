/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Assertions.h>
#include <base/BitCast.h>
#include <base/Forward.h>
#include <base/Noncopyable.h>
#include <base/StdLibExtras.h>

namespace Base {

namespace Detail {
template<typename T, typename Container = RawPtr<T>>
class IntrusiveListNode;

template<typename T, typename Container>
struct SubstituteIntrusiveListNodeContainerType {
    using Type = Container;
};

template<typename T>
struct SubstituteIntrusiveListNodeContainerType<T, NonnullRefPtr<T>> {
    using Type = RefPtr<T>;
};
}

template<typename T, typename Container = RawPtr<T>>
using IntrusiveListNode = Detail::IntrusiveListNode<T, typename Detail::SubstituteIntrusiveListNodeContainerType<T, Container>::Type>;


}