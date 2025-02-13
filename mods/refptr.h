/**
 * @file refptr.h
 * @author Krisna Pranav
 * @brief RefPtr
 * @version 6.0
 * @date 2023-07-02
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#define REFPTR_SCRUB_BYTE 0xe0

#ifdef KERNEL
#include <kernel/library/threadsaferefptr.h>
#else

#include <mods/assertions.h>
#include <mods/atomic.h>
#include <mods/error.h>
#include <mods/format.h>
#include <mods/nonnullrefptr.h>
#include <mods/stdlibextra.h>
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
    class [[nodiscard]] RefPtr
    {
        /**
         * @tparam U 
         * @tparam P 
         */
        template <typename U, typename P>
        friend class RefPtr;
        template <typename U>
        friend class WeakPtr;

    public:
        enum AdoptTag
        {
            Adopt
        }; // enum AdoptTag

        /**
         * @brief Construct a new Ref Ptr object
         * 
         */
        RefPtr() = default;

        /**
         * @brief Construct a new RefPtr object
         * 
         * @param ptr 
         */
        RefPtr(T const* ptr)
            : m_ptr(const_cast<T*>(ptr))
        {
            ref_if_not_null(m_ptr);
        }   

        /**
         * @brief Construct a new RefPtr object
         * 
         * @param object 
         */
        RefPtr(T const& object)
            : m_ptr(const_cast<T*>(&object))
        {
            m_ptr->ref();
        }

        /**
         * @brief Construct a new RefPtr object
         * 
         * @param object 
         */
        RefPtr(AdoptTag, T& object)
            : m_ptr(&object)
        {
        }

        /**
         * @brief Construct a new RefPtr object
         * 
         * @param other 
         */
        RefPtr(RefPtr&& other)
            : m_ptr(other.leak_ref())
        {
        }

        /**
         * @param other 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE RefPtr(NonnullRefPtr<T> const& other)
            : m_ptr(const_cast<T*>(other.ptr()))
        {
            m_ptr->ref();
        }

        /**
         * @tparam U 
         */
        template <typename U>
        ALWAYS_INLINE RefPtr(NonnullRefPtr<U> const& other)
            requires(IsConvertible<U*, T*>)
            : m_ptr(const_cast<T*>(static_cast<T const*>(other.ptr())))
        {
            m_ptr->ref();
        }

        /**
         * @tparam U 
         */
        template <typename U>
        ALWAYS_INLINE RefPtr(NonnullRefPtr<U>&& other)
            requires(IsConvertible<U*, T*>)
            : m_ptr(static_cast<T*>(&other.leak_ref()))
        {
        }

        /**
         * @tparam U 
         * @tparam P 
         */
        template <typename U, typename P = RefPtrTraits<U>>
        RefPtr(RefPtr<U, P>&& other)
            requires(IsConvertible<U*, T*>)
            : m_ptr(static_cast<T*>(other.leak_ref()))
        {
        }

        /**
         * @brief Construct a new RefPtr object
         * 
         * @param other 
         */
        RefPtr(RefPtr const& other)
            : m_ptr(other.m_ptr)
        {
            ref_if_not_null(m_ptr);
        }

        /**
         * @tparam U 
         * @tparam P 
         */
        template <typename U, typename P = RefPtrTraits<U>>
        RefPtr(RefPtr<U, P> const& other)
            requires(IsConvertible<U*, T*>)
            : m_ptr(const_cast<T*>(static_cast<T const*>(other.ptr())))
        {
            ref_if_not_null(m_ptr);
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE ~RefPtr()
        {
            clear();
    #ifdef SANITIZE_PTRS
            m_ptr = reinterpret_cast<T*>(explode_byte(REFPTR_SCRUB_BYTE));
    #endif
        }

        /**
         * @brief Construct a new RefPtr object
         * 
         * @tparam U 
         */
        template <typename U>
        RefPtr(OwnPtr<U> const&) = delete;

        /**
         * @tparam U 
         * @return RefPtr& 
         */
        template <typename U>
        RefPtr& operator=(OwnPtr<U> const&) = delete;

        /**
         * @param other 
         */
        void swap(RefPtr& other)
        {
            Mods::swap(m_ptr, other.m_ptr);
        }

        /**
         * @tparam U 
         * @tparam P 
         */
        template <typename U, typename P = RefPtrTraits<U>>
        void swap(RefPtr<U, P>& other)
            requires(IsConvertible<U*, T*>)
        {
            Mods::swap(m_ptr, other.m_ptr);
        }

        /**
         * @param other 
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE RefPtr& operator=(RefPtr&& other)
        {
            RefPtr tmp{move(other)};
            swap(tmp);
            return *this;
        }

        /**
         * @tparam U 
         * @tparam P 
         */
        template <typename U, typename P = RefPtrTraits<U>>
        ALWAYS_INLINE RefPtr& operator=(RefPtr<U, P>&& other)
            requires(IsConvertible<U*, T*>)
        {
            RefPtr tmp{move(other)};
            swap(tmp);
            return *this;
        }

        /**
         * @tparam U 
         */
        template <typename U>
        ALWAYS_INLINE RefPtr& operator=(NonnullRefPtr<U>&& other)
            requires(IsConvertible<U*, T*>)
        {
            RefPtr tmp{move(other)};
            swap(tmp);
            return *this;
        }

        /**
         * @param other 
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE RefPtr& operator=(NonnullRefPtr<T> const& other)
        {
            RefPtr tmp{other};
            swap(tmp);
            return *this;
        }

        /**
         * @tparam U 
         */
        template <typename U>
        ALWAYS_INLINE RefPtr& operator=(NonnullRefPtr<U> const& other)
            requires(IsConvertible<U*, T*>)
        {
            RefPtr tmp{other};
            swap(tmp);
            return *this;
        }

        /**
         * @param other 
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE RefPtr& operator=(RefPtr const& other)
        {
            RefPtr tmp{other};
            swap(tmp);
            return *this;
        }

        /**
         * @tparam U 
         */
        template <typename U>
        ALWAYS_INLINE RefPtr& operator=(RefPtr<U> const& other)
            requires(IsConvertible<U*, T*>)
        {
            RefPtr tmp{other};
            swap(tmp);
            return *this;
        }

        /**
         * @param ptr 
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE RefPtr& operator=(T const* ptr)
        {
            RefPtr tmp{ptr};
            swap(tmp);
            return *this;
        }

        /**
         * @param object 
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE RefPtr& operator=(T const& object)
        {
            RefPtr tmp{object};
            swap(tmp);
            return *this;
        }

        /**
         * @return RefPtr& 
         */
        RefPtr& operator=(std::nullptr_t)
        {
            clear();
            return *this;
        }

        /**
         * @param other 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE bool assign_if_null(RefPtr&& other)
        {
            if(this == &other)
                return is_null();
            *this = move(other);
            return true;
        }

        /**
         * @tparam U 
         * @tparam P 
         * @param other 
         * @return ALWAYS_INLINE 
         */
        template <typename U, typename P = RefPtrTraits<U>>
        ALWAYS_INLINE bool assign_if_null(RefPtr<U, P>&& other)
        {
            if(this == &other)
                return is_null();
            *this = move(other);
            return true;
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void clear()
        {
            unref_if_not_null(m_ptr);
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
        [[nodiscard]] T* leak_ref()
        {
            return exchange(m_ptr, nullptr);
        }

        /**
         * @return NonnullRefPtr<T> 
         */
        NonnullRefPtr<T> release_nonnull()
        {
            auto* ptr = leak_ref();
            VERIFY(ptr);
            return NonnullRefPtr<T>(NonnullRefPtr<T>::Adopt, *ptr);
        }

        /**
         * @return ALWAYS_INLINE* 
         */
        ALWAYS_INLINE T* ptr()
        {
            return as_ptr();
        }

        /**
         * @return ALWAYS_INLINE const* 
         */
        ALWAYS_INLINE const T* ptr() const
        {
            return as_ptr();
        }

        /**
         * @return ALWAYS_INLINE* 
         */
        ALWAYS_INLINE T* operator->()
        {
            return as_nonnull_ptr();
        }

        /**
         * @return ALWAYS_INLINE const* 
         */
        ALWAYS_INLINE const T* operator->() const
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
         * @return const T* 
         */
        ALWAYS_INLINE operator const T*() const
        {
            return as_ptr();
        }

        /**
         * @return T* 
         */
        ALWAYS_INLINE operator T*()
        {
            return as_ptr();
        }

        /**
         * @return true 
         * @return false 
         */
        ALWAYS_INLINE operator bool()
        {
            return !is_null();
        }

        /**
         * @return true 
         * @return false 
         */
        bool operator==(std::nullptr_t) const
        {
            return is_null();
        }

        /**
         * @return true 
         * @return false 
         */
        bool operator!=(std::nullptr_t) const
        {
            return !is_null();
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(RefPtr const& other) const
        {
            return as_ptr() == other.as_ptr();
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(RefPtr const& other) const
        {
            return as_ptr() != other.as_ptr();
        }   

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(RefPtr& other)
        {
            return as_ptr() == other.as_ptr();
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(RefPtr& other)
        {
            return as_ptr() != other.as_ptr();
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(const T* other) const
        {
            return as_ptr() == other;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(const T* other) const
        {
            return as_ptr() != other;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(T* other)
        {
            return as_ptr() == other;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(T* other)
        {
            return as_ptr() != other;
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE bool is_null() const
        {
            return !m_ptr;
        }

    private:
        /**
         * @return ALWAYS_INLINE* 
         */
        ALWAYS_INLINE T* as_ptr() const
        {
            return m_ptr;
        }

        /**
         * @return ALWAYS_INLINE* 
         */
        ALWAYS_INLINE T* as_nonnull_ptr() const
        {
            VERIFY(m_ptr);
            return m_ptr;
        }

        T* m_ptr{nullptr};
    }; // class [[nodiscard]] RefPtr

    /**
     * @tparam T 
     */
    template <typename T>   
    struct Formatter<RefPtr<T>> : Formatter<const T*>
    {
        /**
         * @param builder 
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> format(FormatBuilder& builder, RefPtr<T> const& value)
        {
            return Formatter<const T*>::format(builder, value.ptr());
        }
    }; // struct Formatter<RefPtr<T>> : Formatter<const T*>

    /**
     * @tparam T 
     */
    template <typename T>
    struct Traits<RefPtr<T>> : public GenericTraits<RefPtr<T>>
    {
        using PeekType = T*;
        using ConstPeekType = const T*;

        /**
         * @param p 
         * @return unsigned 
         */
        static unsigned hash(RefPtr<T> const& p)
        {
            return ptr_hash(p.ptr());
        }

        /**
         * @param a 
         * @param b 
         * @return true 
         * @return false 
         */
        static bool equals(RefPtr<T> const& a, RefPtr<T> const& b)
        {
            return a.ptr() == b.ptr();
        } 
    }; // struct Traits<RefPtr<T>> : public GenericTraits<RefPtr<T>>

    /**
     * @tparam T 
     * @tparam U 
     * @param ptr 
     * @return NonnullRefPtr<T> 
     */
    template <typename T, typename U>
    inline NonnullRefPtr<T> static_ptr_cast(NonnullRefPtr<U> const& ptr)
    {
        return NonnullRefPtr<T>(static_cast<const T&>(*ptr));
    }   

    /**
     * @tparam T 
     * @tparam U 
     * @tparam PtrTraits 
     * @param ptr 
     * @return RefPtr<T> 
     */
    template <typename T, typename U, typename PtrTraits = RefPtrTraits<T>>
    inline RefPtr<T> static_ptr_cast(RefPtr<U> const& ptr)
    {
        return RefPtr<T, PtrTraits>(static_cast<const T*>(ptr.ptr()));
    }

    /**
     * @tparam T 
     * @tparam PtrTraitsT 
     * @tparam U 
     * @tparam PtrTraitsU 
     */
    template <typename T, typename PtrTraitsT, typename U, typename PtrTraitsU>
    inline void swap(RefPtr<T, PtrTraitsT>& a, RefPtr<U, PtrTraitsU>& b)
        requires(IsConvertible<U*, T*>)
    {
        a.swap(b);
    }

    /**
     * @tparam T 
     * @param object 
     * @return RefPtr<T> 
     */
    template <typename T>
    inline RefPtr<T> adopt_ref_if_nonnull(T* object)
    {
        if(object)
            return RefPtr<T>(RefPtr<T>::Adopt, *object);
        return {};
    }

    /**
     * @tparam T 
     * @tparam Args 
     */
    template <typename T, class... Args>
        requires(IsConstructible<T, Args...>)
    inline ErrorOr<NonnullRefPtr<T>> try_make_ref_counted(Args&&... args)
    {
        return adopt_nonnull_ref_or_enomem(new(nothrow) T(forward<Args>(args)...));
    }

    /**
     * @tparam T 
     * @tparam Args 
     * @param args 
     * @return ErrorOr<NonnullRefPtr<T>> 
     */
    template <typename T, class... Args>
    inline ErrorOr<NonnullRefPtr<T>> try_make_ref_counted(Args&&... args)
    {
        return adopt_nonnull_ref_or_enomem(new(nothrow) T{forward<Args>(args)...});
    }

    /**
     * @tparam T 
     * @param object 
     * @return ErrorOr<NonnullRefPtr<T>> 
     */
    template <typename T>
    inline ErrorOr<NonnullRefPtr<T>> adopt_nonnull_ref_or_enomem(T* object)
    {
        auto result = adopt_ref_if_nonnull(object);
        if(!result)
            return Error::from_errno(ENOMEM);
        return result.release_nonnull();
    }

} // namespace Mods

using Mods::adopt_ref_if_nonnull;
using Mod::RefPtr;
using Mods::static_ptr_cast;
using Mods::try_make_ref_counted;

#endif