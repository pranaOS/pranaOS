/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/WeakPtr.h>
#include <libcore/Event.h>
#include <libcore/Object.h>

namespace Core {

ChildEvent::ChildEvent(Type type, Object& child, Object* insertion_before_child)
    : Core::Event(type)
    , m_child(child.make_weak_ptr())
    , m_insertion_before_child(AK::try_make_weak_ptr(insertion_before_child))
{
}

ChildEvent::~ChildEvent()
{
}

Object* ChildEvent::child()
{
    if (auto ref = m_child.strong_ref())
        return ref.ptr();
    return nullptr;
}

const Object* ChildEvent::child() const
{
    if (auto ref = m_child.strong_ref())
        return ref.ptr();
    return nullptr;
}

Object* ChildEvent::insertion_before_child()
{
    if (auto ref = m_insertion_before_child.strong_ref())
        return ref.ptr();
    return nullptr;
}

const Object* ChildEvent::insertion_before_child() const
{
    if (auto ref = m_insertion_before_child.strong_ref())
        return ref.ptr();
    return nullptr;
}

}
