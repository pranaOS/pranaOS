/**
 * @file nonnull_refptr.h
 * @author Krisna Pranav
 * @brief nonnullrefptr
 * @version 6.0
 * @date 2023-07-01
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#define NONNULLREFPTR_SCRUB_BYTE 0xe1

#ifdef KERNEL
#include <kernel/library/threadsafenonnullrefptr.h>
#else
#include <mods/assertions.h>
#include <mods/atomic.h>
#include <mods/format.h>
#include <mods/traits.h>
#include <mods/types.h>

namespace Mods
{
    /**
     * @tparam T 
     */
    template <typename T>
    class OwnPtr;

    /**
     * @tparam T 
     * @tparam PtrTraits 
     */
    template <typename T, typename PtrTraits>
    class RefPtr;

    /**
     * @tparam T 
     * @param ptr 
     * @return ALWAYS_INLINE 
     */
    template <typename T>
    ALWAYS_INLINE void ref_if_not_null(T* ptr)
    {
        if(ptr)
            ptr->ref();
    }

    /**
     * @tparam T 
     * @param ptr 
     * @return ALWAYS_INLINE 
     */
    template <typename T>
    ALWAYS_INLINE void unref_if_not_null(T* ptr)
    {
        if(ptr)
            ptr->unref();
    }

    /**
     * @tparam T 
     */
    template <typename T>
    class [[nodiscard]] NonnullRefPtr
    {
        /**
         * @tparam U 
         * @tparam P 
         */
        template <typename U, typename P>
        friend class RefPtr;

        /**
         * @tparam U 
         */
        template <typename U>
        friend class NonnullRefPtr;

        /**
         * @tparam U 
         */
        template <typename U>
        friend class WeakPtr;

    public:
        using ElementType = T;

        enum AdoptTag
        {
            Adopt
        }; // enum AdoptTag

        /**
         * @param object 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE NonnullRefPtr(T const& object)
            : m_ptr(const_cast<T*>(&object))
        {
            m_ptr->ref();
        }

        /**
         * @tparam U 
         */
        template <typename U>
        ALWAYS_INLINE NonnullRefPtr(U const& object)
            requires(IsConvertible<U*, T*>)
            : m_ptr(const_cast<T*>(static_cast<T const*>(&object)))
        {
            m_ptr->ref();
        }

        /**
         * @param object 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE NonnullRefPtr(AdoptTag, T& object)
            : m_ptr(&object)
        {
        }

        /**
         * @param other 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE NonnullRefPtr(NonnullRefPtr&& other)
            : m_ptr(&other.leak_ref())
        {
        }

        /**
         * @tparam U 
         */
        template <typename U>
        ALWAYS_INLINE NonnullRefPtr(NonnullRefPtr<U>&& other)
            requires(IsConvertible<U*, T*>)
            : m_ptr(static_cast<T*>(&other.leak_ref()))
        {
        }

        /**
         * @param other 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE NonnullRefPtr(NonnullRefPtr const& other)
            : m_ptr(const_cast<T*>(other.ptr()))
        {
            m_ptr->ref();
        }

        /**
         * @tparam U 
         */
        template <typename U>
        ALWAYS_INLINE NonnullRefPtr(NonnullRefPtr<U> const& other)
            requires(IsConvertible<U*, T*>)
            : m_ptr(const_cast<T*>(static_cast<T const*>(other.ptr())))
        {
            m_ptr->ref();
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE ~NonnullRefPtr()
        {
            unref_if_not_null(m_ptr);
            m_ptr = nullptr;
    #ifdef SANITIZE_PTRS
            m_ptr = reinterpret_cast<T*>(explode_byte(NONNULLREFPTR_SCRUB_BYTE));
    #endif
        }

        /**
         * @brief Construct a new Nonnull Ref Ptr object
         * 
         * @tparam U 
         */
        template <typename U>
        NonnullRefPtr(OwnPtr<U> const&) = delete;

        /**
         * @tparam U 
         * @return NonnullRefPtr& 
         */
        template <typename U>
        NonnullRefPtr& operator=(OwnPtr<U> const&) = delete;

        /**
         * @brief Construct a new Nonnull Ref Ptr object
         * 
         * @tparam U 
         */
        template <typename U>
        NonnullRefPtr(RefPtr<U> const&) = delete;

        /**
         * @tparam U 
         * @return NonnullRefPtr& 
         */
        template <typename U>
        NonnullRefPtr& operator=(RefPtr<U> const&) = delete;
        
        NonnullRefPtr(RefPtr<T> const&) = delete;
        NonnullRefPtr& operator=(RefPtr<T> const&) = delete;

        /**
         * @param other 
         * @return NonnullRefPtr& 
         */
        NonnullRefPtr& operator=(NonnullRefPtr const& other)
        {
            NonnullRefPtr tmp{other};
            swap(tmp);
            return *this;
        }   

        /**
         * @tparam U 
         */
        template <typename U>
        NonnullRefPtr& operator=(NonnullRefPtr<U> const& other)
            requires(IsConvertible<U*, T*>)
        {
            NonnullRefPtr tmp{other};
            swap(tmp);
            return *this;
        }

        /**
         * @param other 
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE NonnullRefPtr& operator=(NonnullRefPtr&& other)
        {
            NonnullRefPtr tmp{move(other)};
            swap(tmp);
            return *this;
        }

        /**
         * @tparam U 
         */
        template <typename U>
        NonnullRefPtr& operator=(NonnullRefPtr<U>&& other)
            requires(IsConvertible<U*, T*>)
        {
            NonnullRefPtr tmp{move(other)};
            swap(tmp);
            return *this;
        }

        /**
         * @param object 
         * @return NonnullRefPtr& 
         */
        NonnullRefPtr& operator=(T const& object)
        {
            NonnullRefPtr tmp{object};
            swap(tmp);
            return *this;
        }

        /**
         * @return ALWAYS_INLINE& 
         */
        [[nodiscard]] ALWAYS_INLINE T& leak_ref()
        {
            T* ptr = exchange(m_ptr, nullptr);
            VERIFY(ptr);
            return *ptr;
        }

        /**
         * @return ALWAYS_INLINE* 
         */
        ALWAYS_INLINE RETURNS_NONNULL T* ptr()
        {
            return as_nonnull_ptr();
        }

        /**
         * @return ALWAYS_INLINE const* 
         */
        ALWAYS_INLINE RETURNS_NONNULL const T* ptr() const
        {
            return as_nonnull_ptr();
        }

        /**
         * @return ALWAYS_INLINE* 
         */
        ALWAYS_INLINE RETURNS_NONNULL T* operator->()
        {
            return as_nonnull_ptr();
        }

        /**
         * @return ALWAYS_INLINE const* 
         */
        ALWAYS_INLINE RETURNS_NONNULL const T* operator->() const
        {
            return as_nonnull_ptr();
        }

        /**
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE T& operator*()
        {
            return *as_nonnull_ptr();
        }

        /**
         * @return ALWAYS_INLINE const& 
         */
        ALWAYS_INLINE const T& operator*() const
        {
            return *as_nonnull_ptr();
        }

        /**
         * @return T* 
         */
        ALWAYS_INLINE RETURNS_NONNULL operator T*()
        {
            return as_nonnull_ptr();
        }

        /**
         * @return const T* 
         */
        ALWAYS_INLINE RETURNS_NONNULL operator const T*() const
        {
            return as_nonnull_ptr();
        }

        /**
         * @return T& 
         */
        ALWAYS_INLINE operator T&()
        {
            return *as_nonnull_ptr();
        }

        /**
         * @return const T& 
         */
        ALWAYS_INLINE operator const T&() const
        {
            return *as_nonnull_ptr();
        }

        operator bool() const = delete;
        bool operator!() const = delete;

        /**
         * @param other 
         */
        void swap(NonnullRefPtr& other)
        {
            Mods::swap(m_ptr, other.m_ptr);
        }

        /**

         * @tparam U 
         */
        template <typename U>
        void swap(NonnullRefPtr<U>& other)
            requires(IsConvertible<U*, T*>)
        {
            Mods::swap(m_ptr, other.m_ptr);
        }

    private:
        NonnullRefPtr() = delete;
        
        /**
         * @return ALWAYS_INLINE* 
         */
        ALWAYS_INLINE RETURNS_NONNULL T* as_nonnull_ptr() const
        {
            VERIFY(m_ptr);
            return m_ptr;
        }

        T* m_ptr{nullptr};
    }; // class [[nodiscard]] NonnullRefPtr

    /**
     * @tparam T 
     * @param object 
     * @return NonnullRefPtr<T> 
     */
    template <typename T>
    inline NonnullRefPtr<T> adopt_ref(T& object)
    {
        return NonnullRefPtr<T>(NonnullRefPtr<T>::Adopt, object);
    }

    /**
     * @tparam T 
     */
    template <typename T>
    struct Formatter<NonnullRefPtr<T>> : Formatter<const T*>
    {
        /**
         * @param builder 
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> format(FormatBuilder& builder, NonnullRefPtr<T> const& value)
        {
            return Formatter<const T*>::format(builder, value.ptr());
        }
    };

    /**
     * @tparam T 
     * @tparam U 
     */
    template <typename T, typename U>
    inline void swap(NonnullRefPtr<T>& a, NonnullRefPtr<U>& b)
        requires(IsConvertible<U*, T*>)
    {
        a.swap(b);
    }

    /**
     * @tparam T 
     * @tparam Args 
     */
    template <typename T, class... Args>
        requires(IsConstructible<T, Args...>)
    inline NonnullRefPtr<T> make_ref_counted(Args&&... args)
    {
        return NonnullRefPtr<T>(NonnullRefPtr<T>::Adopt, *new T(forward<Args>(args)...));
    }

    /**
     * @tparam T 
     * @tparam Args 
     * @param args 
     * @return NonnullRefPtr<T> 
     */
    template <typename T, class... Args>
    inline NonnullRefPtr<T> make_ref_counted(Args&&... args)
    {
        return NonnullRefPtr<T>(NonnullRefPtr<T>::Adopt, *new T{forward<Args>(args)...});
    }
} // namespace Mods

/**
 * @tparam T 
 */
template <typename T>
struct Traits<NonnullRefPtr<T>> : public GenericTraits<NonnullRefPtr<T>>
{
    using PeekType = T*;
    using ConstPeekType = const T*;
    static unsigned hash(NonnullRefPtr<T> const& p)
    {
        return ptr_hash(p.ptr());
    }
    static bool equals(NonnullRefPtr<T> const& a, NonnullRefPtr<T> const& b)
    {
        return a.ptr() == b.ptr();
    }
};

using Mods::adopt_ref;
using Mods::make_ref_counted;
using Mods::NonnullRefPtr;

#endif