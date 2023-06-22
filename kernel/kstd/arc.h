/**
* @file waitblocker.h
* @author Krisna Pranav
* @brief WaitBlocker
* @version 1.0
*
* @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
*
 */

#pragma once

#include "refcount.h"

namespace kstd {
    template<typename T>
    class Weak;

    template<typename T>
    class ArcSelf;
    class ArcSelfBase;

    template<typename T>
    void __set_shared_weak_self(ArcSelfBase* base, Weak<T> weak);

    template<typename T>
    class Arc {
      public:
        Arc(): m_ptr(nullptr), m_count(nullptr) {}

        template <typename U>
        Arc(const Arc<U>& other):
            m_ptr(static_cast<T*>(other.m_ptr)),
            m_count(other.m_count)
        {
            if(m_count)
                m_count->acquire_strong();
        }
    };
}