/**
 * @file refcounted.h
 * @author Krisna Pranav
 * @brief RefCounted
 * @version 6.0
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

    /**
     * @tparam T 
     * @param object 
     * @return decltype(const_cast<T*>(object)->will_be_destroyed(), TrueType {}) 
     */
    template<class T>
    constexpr auto call_will_be_destroyed_if_present(const T* object) -> decltype(const_cast<T*>(object)->will_be_destroyed(), TrueType {}) {
        const_cast<T*>(object)->will_be_destroyed();
        return {};
    }

    /**
     * @param ... 
     * @return FalseType 
     */
    constexpr auto call_will_be_destroyed_if_present(...) -> FalseType {
        return {};
    }

    /**
     * @tparam T 
     * @param object 
     * @return decltype(const_cast<T*>(object)->one_ref_left(), TrueType {}) 
     */
    template<class T>
    constexpr auto call_one_ref_left_if_present(const T* object) -> decltype(const_cast<T*>(object)->one_ref_left(), TrueType {}) {
        const_cast<T*>(object)->one_ref_left();
        return {};
    }

    /**
     * @param ... 
     * @return FalseType 
     */
    constexpr auto call_one_ref_left_if_present(...) -> FalseType {
        return {};
    }

    class RefCountedBase {
        MOD_MAKE_NONCOPYABLE(RefCountedBase);
        MOD_MAKE_NONMOVABLE(RefCountedBase);

    public:
        using RefCountType = unsigned int;
        using AllowOwnPtr = FalseType;

        /**
         * @brief ref
         * 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void ref() const {
            auto old_ref_count = m_ref_count.fetch_add(1, Mods::MemoryOrder::memory_order_relaxed);
            ASSERT(old_ref_count > 0);
            ASSERT(!Checked<RefCountType>::addition_would_overflow(old_ref_count, 1));
        }

        /**
         * @brief ref_count
         * 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE RefCountType ref_count() const {
            return m_ref_count.load(Mods::MemoryOrder::memory_order_relaxed);
        }

    protected:
        RefCountedBase() { }
        ALWAYS_INLINE ~RefCountedBase() {
            ASSERT(m_ref_count.load(Mods::MemoryOrder::memory_order_relaxed) == 0);
        }

        ALWAYS_INLINE RefCountType deref_base() const {
            auto old_ref_count = m_ref_count.fetch_sub(1, Mods::MemoryOrder::memory_order_acq_rel);
            ASSERT(old_ref_count > 0);
            return old_ref_count - 1;
        }

        mutable Atomic<RefCountType> m_ref_count { 1 };
    };

    /**
     * @tparam T 
     */
    template<typename T>
    class RefCounted : public RefCountedBase {
    public:
        void unref() const {
            auto new_ref_count = deref_base();
            if (new_ref_count == 0) {
                call_will_be_destroyed_if_present(static_cast<const T*>(this));
                delete static_cast<const T*>(this);
            } else if (new_ref_count == 1) {
                call_one_ref_left_if_present(static_cast<const T*>(this));
            }
        }
    };

}

using Mods::RefCounted;
