/**
 * @file nonnullownptr.h
 * @author Krisna Pranav
 * @brief nonnullownptr.h
 * @version 6.0
 * @date 2023-07-30
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/assertions.h>
#include <mods/format.h>
#include <mods/refcounted.h>
#include <mods/stdlibextra.h>
#include <mods/traits.h>
#include <mods/types.h>

#define NONNULLOWNPTR_SCRUB_BYTE 0xf1

namespace Mods
{
    /**
     * @tparam T 
     * @tparam PtrTraits 
     */
    template <typename T, typename PtrTraits>
    class RefPtr;

    /**
     * @tparam T 
     */
    template <typename T>
    class NonnullRefPtr;

    /**
     * @tparam T 
     */
    template <typename T>
    class WeakPtr;

    /**
     * @tparam T 
     */
    template <typename T>
    class [[nodiscard]] NonnullOwnPtr
    {
    public:
        using ElementType = T;

        enum AdoptTag
        {
            Adopt
        }; // enum AdoptTag

        /**
         * @brief Construct a new Nonnull Own Ptr object
         * 
         * @param ptr 
         */
        NonnullOwnPtr(AdoptTag, T& ptr)
            : m_ptr(&ptr)
        {
            static_assert(
                requires { requires typename T::AllowOwnPtr()(); } || !requires { requires !typename T::AllowOwnPtr()(); declval<T>().ref(); declval<T>().unref(); },
                "Use NonnullRefPtr<> for RefCounted types");
        }

        /**
         * @brief Construct a new Nonnull Own Ptr object
         * 
         * @param other 
         */
        NonnullOwnPtr(NonnullOwnPtr&& other)
            : m_ptr(other.leak_ptr())
        {
            VERIFY(m_ptr);
        }

        /**
         * @brief Construct a new Nonnull Own Ptr object
         * 
         * @tparam U 
         * @param other 
         */
        template <typename U>
        NonnullOwnPtr(NonnullOwnPtr<U>&& other)
            : m_ptr(other.leak_ptr())
        {
            VERIFY(m_ptr);
        }

        /**
         * @brief Destroy the Nonnull Own Ptr object
         * 
         */
        ~NonnullOwnPtr()
        {
            clear();
    #ifdef SANITIZE_PTRS
            m_ptr = (T*)(explode_byte(NONNULLOWNPTR_SCRUB_BYTE));
    #endif
        }   

        /**
         * @brief Construct a new Nonnull Own Ptr object
         * 
         */
        NonnullOwnPtr(NonnullOwnPtr const&) = delete;
        template <typename U>
        NonnullOwnPtr(NonnullOwnPtr<U> const&) = delete;
        NonnullOwnPtr& operator=(NonnullOwnPtr const&) = delete;
        template <typename U>
        NonnullOwnPtr& operator=(NonnullOwnPtr<U> const&) = delete;

        /**
         * @brief Construct a new Nonnull Own Ptr object
         * 
         * @tparam U 
         * @tparam PtrTraits 
         */
        template <typename U, typename PtrTraits = RefPtrTraits<U>>
        NonnullOwnPtr(RefPtr<U, PtrTraits> const&) = delete;
        template <typename U>
        NonnullOwnPtr(NonnullRefPtr<U> const&) = delete;
        template <typename U>
        NonnullOwnPtr(WeakPtr<U> const&) = delete;
        template <typename U, typename PtrTraits = RefPtrTraits<U>>
        NonnullOwnPtr& operator=(RefPtr<U, PtrTraits> const&) = delete;
        template <typename U>
        NonnullOwnPtr& operator=(NonnullRefPtr<U> const&) = delete;
        template <typename U>
        NonnullOwnPtr& operator=(WeakPtr<U> const&) = delete;

        /**
         * @param other 
         * @return NonnullOwnPtr& 
         */
        NonnullOwnPtr& operator=(NonnullOwnPtr&& other)
        {
            NonnullOwnPtr ptr(move(other));
            swap(ptr);
            return *this;
        }

        /**
         * @tparam U 
         * @param other 
         * @return NonnullOwnPtr& 
         */
        template <typename U>
        NonnullOwnPtr& operator=(NonnullOwnPtr<U>&& other)
        {
            NonnullOwnPtr ptr(move(other));
            swap(ptr);
            return *this;
        }

        /**
         * @return T* 
         */
        [[nodiscard]] T* leak_ptr()
        {
            return exchange(m_ptr, nullptr);
        }

        /**
         * @return ALWAYS_INLINE* 
         */
        ALWAYS_INLINE RETURNS_NONNULL T* ptr()
        {
            VERIFY(m_ptr);
            return m_ptr;
        }

        /**
         * @return ALWAYS_INLINE const* 
         */
        ALWAYS_INLINE RETURNS_NONNULL const T* ptr() const
        {
            VERIFY(m_ptr);
            return m_ptr;
        }

        /**
         * @return ALWAYS_INLINE* 
         */
        ALWAYS_INLINE RETURNS_NONNULL T* operator->()
        {
            return ptr();
        }

        /**
         * @return ALWAYS_INLINE const* 
         */
        ALWAYS_INLINE RETURNS_NONNULL const T* operator->() const
        {
            return ptr();
        }

        /**
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE T& operator*()
        {
            return *ptr();
        }

        /**
         * @return ALWAYS_INLINE const& 
         */
        ALWAYS_INLINE const T& operator*() const
        {
            return *ptr();
        }

        /**
         * @return const T* 
         */
        ALWAYS_INLINE RETURNS_NONNULL operator const T*() const
        {
            return ptr();
        }

        /**
         * @return T* 
         */
        ALWAYS_INLINE RETURNS_NONNULL operator T*()
        {
            return ptr();
        }

        operator bool() const = delete;
        bool operator!() const = delete;

        /**
         * @param other 
         */
        void swap(NonnullOwnPtr& other)
        {
            ::swap(m_ptr, other.m_ptr);
        }

        /**
         * @tparam U 
         * @param other 
         */
        template <typename U>
        void swap(NonnullOwnPtr<U>& other)
        {
            ::swap(m_ptr, other.m_ptr);
        }

        /**
         * @tparam U 
         * @return NonnullOwnPtr<U> 
         */
        template <typename U>
        NonnullOwnPtr<U> release_nonnull()
        {
            VERIFY(m_ptr);
            return NonnullOwnPtr<U>(NonnullOwnPtr<U>::Adopt, static_cast<U&>(*leak_ptr()));
        }

    private:
        void clear()
        {
            if(!m_ptr)
                return;
            delete m_ptr;
            m_ptr = nullptr;
        }

        T* m_ptr = nullptr;
    }; // class [[nodiscard]] NonnullOwnPtr

    #if !defined(KERNEL)

    /**
     * @tparam T 
     * @param object 
     * @return NonnullOwnPtr<T> 
     */
    template <typename T>
    inline NonnullOwnPtr<T> adopt_own(T& object)
    {
        return NonnullOwnPtr<T>(NonnullOwnPtr<T>::Adopt, object);
    }

    /**
     * @tparam T 
     * @tparam Args 
     */
    template <class T, class... Args>
        requires(IsConstructible<T, Args...>)
    inline NonnullOwnPtr<T> make(Args&&... args)
    {
        return NonnullOwnPtr<T>(NonnullOwnPtr<T>::Adopt, *new T(forward<Args>(args)...));
    }

    /**
     * @tparam T 
     * @tparam Args 
     * @param args 
     * @return NonnullOwnPtr<T> 
     */
    template <class T, class... Args>
    inline NonnullOwnPtr<T> make(Args&&... args)
    {
        return NonnullOwnPtr<T>(NonnullOwnPtr<T>::Adopt, *new T{forward<Args>(args)...});
    }

    #endif

    /**
     * @tparam T 
     */
    template <typename T>
    struct Traits<NonnullOwnPtr<T>> : public GenericTraits<NonnullOwnPtr<T>>
    {
        using PeekType = T*;
        using ConstPeekType = const T*;
        static unsigned hash(NonnullOwnPtr<T> const& p)
        {
            return ptr_hash((FlatPtr)p.ptr());
        }
        static bool equals(NonnullOwnPtr<T> const& a, NonnullOwnPtr<T> const& b)
        {
            return a.ptr() == b.ptr();
        }
    };

    /**
     * @tparam T 
     * @tparam U 
     * @param a 
     * @param b 
     */
    template <typename T, typename U>
    inline void swap(NonnullOwnPtr<T>& a, NonnullOwnPtr<U>& b)
    {
        a.swap(b);
    }

    /**
     * @tparam T 
     */
    template <typename T>
    struct Formatter<NonnullOwnPtr<T>> : Formatter<const T*>
    {
        ErrorOr<void> format(FormatBuilder& builder, NonnullOwnPtr<T> const& value)
        {
            return Formatter<const T*>::format(builder, value.ptr());
        }
    };

} // namespace Mods

#if !defined(KERNEL)
using Mods::adopt_own;
using Mods::make;
#endif
using Mods::NonnullOwnPtr;
