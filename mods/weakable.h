/**
 * @file weakable.h
 * @author Krisna Pranav
 * @brief weakable
 * @version 1.0
 * @date 2023-07-11
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include "assertions.h"
#include "atomic.h"
#include "refcounted.h"
#include "refptr.h"

#ifndef WEAKABLE_DEBUG
#   define WEAKABLE_DEBUG
#endif 

namespace Mods
{
    template<typename T>
    class Weakable;

    template<typename T>
    class WeakPtr;

    class WeakLink: public RefCounted<WeakLink>
    {
        template<typename T>
        friend class Weakable;

        template<typename T>
        friend class WeakPtr;

    public:

    private:
        template<typename T>
        explicit WeakLink(T& weakable) 
            : m_bits (RefPtrTraits<void>(&weakable))
        {}

        mutable Atomic<FlatPtr> m_bits;
    };

    template<typename T>
    class Weakable
    {
    private:
        class Link;

    protected:
        Weakable() {}

        ~Weakable()
        {
        #ifdef WEAKABLE_DEBUG
            m_being_destroyed = true;
        #endif
        }

        void revoke_weak_ptr()
        {
            if (m_link)
                m_link->revoke();
        }

    private:
        mutable RefPtr<WeakLink> m_link;

        #ifdef WEAKABLE_DEBUG
            bool m_being_destroyed { false };
        #endif 
    };
}