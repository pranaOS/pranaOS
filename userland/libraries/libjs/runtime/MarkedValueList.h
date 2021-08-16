/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/IntrusiveList.h>
#include <base/Noncopyable.h>
#include <base/Vector.h>
#include <libjs/Forward.h>
#include <libjs/runtime/Value.h>

namespace JS {

class MarkedValueList : public Vector<Value, 32> {
    BASE_MAKE_NONCOPYABLE(MarkedValueList);

public:
    explicit MarkedValueList(Heap&);
    MarkedValueList(MarkedValueList&&);
    ~MarkedValueList();

    MarkedValueList& operator=(MarkedValueList&&) = delete;

    Vector<Value, 32>& values() { return *this; }

    MarkedValueList copy() const
    {
        MarkedValueList copy { m_heap };
        copy.extend(*this);
        return copy;
    }

private:
    Heap& m_heap;

    IntrusiveListNode<MarkedValueList> m_list_node;

public:
    using List = IntrusiveList<MarkedValueList, RawPtr<MarkedValueList>, &MarkedValueList::m_list_node>;
};

}
