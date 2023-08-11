/**
 * @file weakptr.h
 * @author Krisna Pranav
 * @brief weakptr
 * @version 1.0
 * @date 2023-08-11
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/logstream.h>
#include <mods/weakable.h>

namespace Mods
{
    template<typename T>
    class WeakPtr
    {
        template<typename U>
        friend class Weakable;

    public:
        WeakPtr() {}

        WeakPtr(std::nullptr_t) {}

        template<typename U, typename EnableIf<IsBaseOf<T, U>::value>::Type* = nullptr>
        WeakPtr(const WeakPtr<U>& other)
            : m_link(other.m_link)
        {}

    private:
        WeakPtr(const RefPtr<WeakLink>& link)
            : m_link(link)
        {}

        RefPtr<WeakLink> m_link;
    }; // class WeakPtr

    /**
     * @tparam T 
     * @tparam U 
     * @return WeakPtr<U> 
     */
    template<typename T>
    template<typename U>
    inline WeakPtr<U> Weakable<T>::make_weak_ptr() const
    {
        #ifdef DEBUG
            ASSERT(!m_being_destroyed);
        #endif

        if (!m_link) {
            m_link.assign_if_null(adopt(*new WeakLink()));
        }
    }

} // namespace Mods