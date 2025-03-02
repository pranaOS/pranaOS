/**
 * @file ownptr.h
 * @author Krisna Pranav
 * @brief ownptr
 * @version 6.0
 * @date 2023-07-10
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/error.h>
#include <mods/nonnullownptr.h>
#include <mods/refcounted.h>

#define OWNPTR_SCRUB_BYTE 0xf0

namespace Mods
{
    /**
     * @tparam T 
     */
    template <typename T>
    class [[nodiscard]] OwnPtr
    {
    public:
        OwnPtr() = default;

        /**
         * @brief Construct a new Own Ptr object
         * 
         */
        OwnPtr(decltype(nullptr))
            : m_ptr(nullptr)
        {
        }

        /**
         * @brief Construct a new Own Ptr object
         * 
         * @param other 
         */
        OwnPtr(OwnPtr&& other)
            : m_ptr(other.leak_ptr())
        {
        }   

        /**
         * @brief Construct a new Own Ptr object
         * 
         * @tparam U 
         * @param other 
         */
        template <typename U>
        OwnPtr(NonnullOwnPtr<U>&& other)
            : m_ptr(other.leak_ptr())
        {
        }

        /**
         * @brief Construct a new Own Ptr object
         * 
         * @tparam U 
         * @param other 
         */
        template <typename U>
        OwnPtr(OwnPtr<U>&& other)
            : m_ptr(other.leak_ptr())
        {
        }

        /**
         * @brief Destroy the Own Ptr object
         * 
         */
        ~OwnPtr()
        {
            clear();
    #ifdef SANITIZE_PTRS
            m_ptr = (T*)(explode_byte(OWNPTR_SCRUB_BYTE));
    #endif
        }

        OwnPtr(OwnPtr const&) = delete;
        template <typename U>
        OwnPtr(OwnPtr<U> const&) = delete;
        OwnPtr& operator=(OwnPtr const&) = delete;
        template <typename U>
        OwnPtr& operator=(OwnPtr<U> const&) = delete;

        template <typename U>
        OwnPtr(NonnullOwnPtr<U> const&) = delete;
        template <typename U>
        OwnPtr& operator=(NonnullOwnPtr<U> const&) = delete;
        template <typename U>
        OwnPtr(RefPtr<U> const&) = delete;
        template <typename U>
        OwnPtr(NonnullRefPtr<U> const&) = delete;
        template <typename U>
        OwnPtr(WeakPtr<U> const&) = delete;
        template <typename U>
        OwnPtr& operator=(RefPtr<U> const&) = delete;
        /**
         * @tparam U 
         * @return OwnPtr& 
         */
        template <typename U>
        OwnPtr& operator=(NonnullRefPtr<U> const&) = delete;

        /**
         * @tparam U 
         * @return OwnPtr& 
         */
        template <typename U>
        OwnPtr& operator=(WeakPtr<U> const&) = delete;

        /**
         * @param other 
         * @return OwnPtr& 
         */
        OwnPtr& operator=(OwnPtr&& other)
        {
            OwnPtr ptr(move(other));
            swap(ptr);
            return *this;
        }

        /**
         * @tparam U 
         * @param other 
         * @return OwnPtr& 
         */
        template <typename U>
        OwnPtr& operator=(OwnPtr<U>&& other)
        {
            OwnPtr ptr(move(other));
            swap(ptr);
            return *this;
        }

        /**
         * @tparam U 
         * @param other 
         * @return OwnPtr& 
         */
        template <typename U>
        OwnPtr& operator=(NonnullOwnPtr<U>&& other)
        {
            OwnPtr ptr(move(other));
            swap(ptr);
            VERIFY(m_ptr);
            return *this;
        }

        /**
         * @param ptr 
         * @return OwnPtr& 
         */
        OwnPtr& operator=(T* ptr) = delete;

        /**
         * @return OwnPtr& 
         */
        OwnPtr& operator=(std::nullptr_t)
        {
            clear();
            return *this;
        }

        void clear()
        {
            delete m_ptr;
            m_ptr = nullptr;
        }

        /**
         * @return true 
         * @return false 
         */
        bool operator!() const
        {
            return !m_ptr;
        }

        /**
         * @return T* 
         */
        [[nodiscard]] T* leak_ptr()
        {
            T* leaked_ptr = m_ptr;
            m_ptr = nullptr;
            return leaked_ptr;
        }

        /**
         * @return NonnullOwnPtr<T> 
         */
        NonnullOwnPtr<T> release_nonnull()
        {
            VERIFY(m_ptr);
            return NonnullOwnPtr<T>(NonnullOwnPtr<T>::Adopt, *leak_ptr());
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

        /**
         * @return T* 
         */
        T* ptr()
        {
            return m_ptr;
        }

        /**
         * @return const T* 
         */
        const T* ptr() const
        {
            return m_ptr;
        }

        /**
         * @return T* 
         */
        T* operator->()
        {
            VERIFY(m_ptr);
            return m_ptr;
        }

        /**
         * @return const T* 
         */
        const T* operator->() const
        {
            VERIFY(m_ptr);
            return m_ptr;
        }

        /**
         * @return T& 
         */
        T& operator*()
        {
            VERIFY(m_ptr);
            return *m_ptr;
        }

        /**
         * @return const T& 
         */
        const T& operator*() const
        {
            VERIFY(m_ptr);
            return *m_ptr;
        }

        /**
         * @return const T* 
         */
        operator const T*() const
        {
            return m_ptr;
        }

        /**
         * @return T* 
         */
        operator T*()
        {
            return m_ptr;
        }

        /**
         * @return true 
         * @return false 
         */
        operator bool()
        {
            return !!m_ptr;
        }   

        /**
         * @param other 
         */
        void swap(OwnPtr& other)
        {
            ::swap(m_ptr, other.m_ptr);
        }

        /**
         * @tparam U 
         * @param other 
         */
        template <typename U>
        void swap(OwnPtr<U>& other)
        {
            ::swap(m_ptr, other.m_ptr);
        }

        /**
         * @param ptr 
         * @return OwnPtr 
         */
        static OwnPtr lift(T* ptr)
        {
            return OwnPtr{ptr};
        }

    protected:
        /**
         * @brief Construct a new Own Ptr object
         * 
         * @param ptr 
         */
        explicit OwnPtr(T* ptr)
            : m_ptr(ptr)
        {
            static_assert(
                requires { requires typename T::AllowOwnPtr()(); } || !requires { requires !typename T::AllowOwnPtr()(); declval<T>().ref(); declval<T>().unref(); }, "Use RefPtr<> for RefCounted types");
        }

    private:
        T* m_ptr = nullptr;
    }; // class [[nodiscard]] OwnPtr

    /**
     * @tparam T 
     * @tparam U 
     * @param a 
     * @param b 
     */
    template <typename T, typename U>
    inline void swap(OwnPtr<T>& a, OwnPtr<U>& b)
    {
        a.swap(b);
    }

    /**
     * @tparam T 
     * @param object 
     * @return OwnPtr<T> 
     */
    template <typename T>
    inline OwnPtr<T> adopt_own_if_nonnull(T* object)
    {
        if(object)
            return OwnPtr<T>::lift(object);
        return {};
    }

    /**
     * @tparam T 
     * @param object 
     * @return ErrorOr<NonnullOwnPtr<T>> 
     */
    template <typename T>
    inline ErrorOr<NonnullOwnPtr<T>> adopt_nonnull_own_or_enomem(T* object)
    {
        auto result = adopt_own_if_nonnull(object);
        if(!result)
            return Error::from_errno(ENOMEM);
        return result.release_nonnull();
    }

    /**
     * @tparam T 
     * @tparam Args 
     */
    template <typename T, class... Args>
        requires(IsConstructible<T, Args...>)
    inline ErrorOr<NonnullOwnPtr<T>> try_make(Args&&... args)
    {
        return adopt_nonnull_own_or_enomem(new(nothrow) T(forward<Args>(args)...));
    }

    /**
     * @tparam T 
     * @tparam Args 
     * @param args 
     * @return ErrorOr<NonnullOwnPtr<T>> 
     */
    template <typename T, class... Args>
    inline ErrorOr<NonnullOwnPtr<T>> try_make(Args&&... args)
    {
        return adopt_nonnull_own_or_enomem(new(nothrow) T{forward<Args>(args)...});
    }

    /**
     * @tparam T 
     */
    template <typename T>
    struct Traits<OwnPtr<T>> : public GenericTraits<OwnPtr<T>>
    {
        using PeekType = T*;
        using ConstPeekType = const T*;
        static unsigned hash(OwnPtr<T> const& p)
        {
            return ptr_hash(p.ptr());
        }
        static bool equals(OwnPtr<T> const& a, OwnPtr<T> const& b)
        {
            return a.ptr() == b.ptr();
        }
    };

} // namespace Mods

using Mods::adopt_nonnull_own_or_enomem;
using Mods::adopt_own_if_nonnull;
using Mods::OwnPtr;
using Mods::try_make;