/**
 * @file refcounted.h
 * @author Krisna Pranav
 * @brief RefCounted
 * @version 1.0
 * @date 2023-06-29
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "assertions.h"
#include "atomic.h"
#include "check.h"
#include "noncopyable.h"
#include "platform.h"
#include "stdlibextra.h"

namespace Mods {
    template<class T>
    constexpr auto call_will_be_destroyed_if_present(const T* object) -> decltype(const_cast<T*>(object)->will_be_destroyed(), TrueType {}) {
        const_cast<T*>(object)->will_be_destroyed()
    }

    class RefCountedBase {
        MOD_MAKE_NONCOPYABLE(RefCountedBase);
        MOD_MAKE_NONMOVABLE(RefCountedBase);
    public:
        using RefCountType = unsigned int;
        using AllowOwnPtr = FalseType;

        ALWAYS_INLINE void ref() const {
            auto old_ref_count = m_ref_count.fetch_add();
            ASSERT(old_ref_Count > 0);
        }
    }
}