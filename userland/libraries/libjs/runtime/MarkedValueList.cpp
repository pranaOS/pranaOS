/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/heap/Heap.h>
#include <libjs/runtime/MarkedValueList.h>

namespace JS {

MarkedValueList::MarkedValueList(Heap& heap)
    : m_heap(heap)
{
    m_heap.did_create_marked_value_list({}, *this);
}

MarkedValueList::MarkedValueList(MarkedValueList&& other)
    : Vector<Value, 32>(move(static_cast<Vector<Value, 32>&>(other)))
    , m_heap(other.m_heap)
{
    m_heap.did_create_marked_value_list({}, *this);
}

MarkedValueList::~MarkedValueList()
{
    m_heap.did_destroy_marked_value_list({}, *this);
}

}
