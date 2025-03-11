/**
 * @file event.cpp
 * @author Krisna Pranav
 * @brief event
 * @version 6.0
 * @date 2025-02-28
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/weakptr.h>
#include <libcore/event.h>
#include <libcore/object.h>

namespace Core 
{

    /**
     * @brief Construct a new ChildEvent::ChildEvent object
     * 
     * @param type 
     * @param child 
     * @param insertion_before_child 
     */
    ChildEvent::ChildEvent(Type type, Object& child, Object* insertion_before_child)
        : Core::Event(type)
        , m_child(child.make_weak_ptr())
        , m_insertion_before_child(Mods::make_weak_ptr_if_nonnull(insertion_before_child))
    {
    }

    /**
     * @return Object* 
     */
    Object* ChildEvent::child()
    {
        if (auto ref = m_child.strong_ref())
            return ref.ptr();

        return nullptr;
    }

    /**
     * @return Object const* 
     */
    Object const* ChildEvent::child() const
    {
        if (auto ref = m_child.strong_ref())
            return ref.ptr();

        return nullptr;
    }

    /**
     * @return Object* 
     */
    Object* ChildEvent::insertion_before_child()
    {
        if (auto ref = m_insertion_before_child.strong_ref())
            return ref.ptr();

        return nullptr;
    }

    /**
     * @return Object const* 
     */
    Object const* ChildEvent::insertion_before_child() const
    {
        if (auto ref = m_insertion_before_child.strong_ref())
            return ref.ptr();

        return nullptr;
    }

} // namespace Core
