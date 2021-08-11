/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <base/IntrusiveList.h>

namespace Base {

template<class T, typename Container, IntrusiveListNode<T, Container> T::*member>
class IntrusiveListRelaxedConst : public IntrusiveList<T, Container, member> {
    BASE_MAKE_NONCOPYABLE(IntrusiveListRelaxedConst);
    BASE_MAKE_NONMOVABLE(IntrusiveListRelaxedConst);

public:
    using IntrusiveList<T, Container, member>::IntrusiveList;

    using Iterator = typename IntrusiveList<T, Container, member>::Iterator;

    Iterator begin() const { return const_cast<IntrusiveListRelaxedConst*>(this)->IntrusiveList<T, Container, member>::begin(); }
    Iterator end() const { return Iterator {}; }
};

}

using Base::IntrusiveListRelaxedConst;
