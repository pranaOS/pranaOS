/**
 * @file refcounted.h
 * @author Krisna Pranav
 * @brief RefCounted
 * @version 6.0
 * @date 2023-06-29
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#ifdef KERNEL
#include <kernel/library/threadsaferefcounted.h>
#else

#include <mods/assertions.h>
#include <mods/checked.h>
#include <mods/noncopyable.h>
#include <mods/platform.h>
#include <mods/stdlibextra.h>

namespace Mods
{
    class RefCountedBase
    {
        MOD_MAKE_NONCOPYABLE(RefCountedBase);
        MOD_MAKE_NONMOVABLE(RefCountedBase);

    public:
        using RefCountType = unsigned int;
        using AllowOwnPtr = FalseType;

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void ref() const
        {
            VERIFY(m_ref_count > 0);
            VERIFY(!Checked<RefCountType>::addition_would_overflow(m_ref_count, 1));
            ++m_ref_count;
        }

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool try_ref() const
        {
            if(m_ref_count == 0)
                return false;

            ref();
            return true;
        }

        /**
         * @return RefCountType 
         */
        [[nodiscard]] RefCountType ref_count() const
        {
            return m_ref_count;
        }

    protected:
        /**
         * @brief Construct a new Ref Counted Base object
         * 
         */
        RefCountedBase() = default;

        /**
         * @brief Destroy the Ref Counted Base object
         * 
         */
        ~RefCountedBase()
        {
            VERIFY(!m_ref_count);
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE RefCountType deref_base() const
        {
            VERIFY(m_ref_count);
            return --m_ref_count;
        }

        RefCountType mutable m_ref_count{1};
    }; // class RefCountedBase

    /**
     * @tparam T 
     */
    template <typename T>
    class RefCounted : public RefCountedBase
    {
    public:
        bool unref() const
        {
            auto* that = const_cast<T*>(static_cast<T const*>(this));

            auto new_ref_count = deref_base();
            if(new_ref_count == 0)
            {
                if constexpr(requires { that->will_be_destroyed(); })
                    that->will_be_destroyed();
                delete static_cast<const T*>(this);
                return true;
            }
            return false;
        }
    }; // class RefCounted : public RefCountedBase

} // namespace Mods

using Mods::RefCounted;
using Mods::RefCountedBase;

#endif
