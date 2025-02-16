/**
 * @file weakptr.h
 * @author Krisna Pranav
 * @brief weakptr
 * @version 6.0
 * @date 2023-08-11
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#ifdef KERNEL
#include <kernel/library/threadsafeweakptr.h>
#else

#include <mods/weakable.h>

namespace Mods
{
    /**
     * @tparam T 
     */
    template <typename T>
    class [[nodiscard]] WeakPtr
    {
        template <typename U>
        friend class Weakable;

    public:
        /**
         * @brief Construct a new Weak Ptr object
         * 
         */
        WeakPtr() = default;

        /**
         * @tparam U 
         */
        template <typename U>
        WeakPtr(WeakPtr<U> const& other)
            requires(IsBaseOf<T, U>)
            : m_link(other.m_link)
        {
        }

        /**
         * @tparam U 
         */
        template <typename U>
        WeakPtr(WeakPtr<U>&& other)
            requires(IsBaseOf<T, U>)
            : m_link(other.take_link())
        {
        }

        /**
         * @tparam U 
         */
        template <typename U>
        WeakPtr& operator=(WeakPtr<U>&& other)
            requires(IsBaseOf<T, U>)
        {
            m_link = other.take_link();
            return *this;
        }

        /**
         * @tparam U 
         */
        template <typename U>
        WeakPtr& operator=(WeakPtr<U> const& other)
            requires(IsBaseOf<T, U>)
        {
            if((void const*)this != (void const*)&other)
                m_link = other.m_link;
            return *this;
        }

        /**
         * @return WeakPtr& 
         */
        WeakPtr& operator=(std::nullptr_t)
        {
            clear();
            return *this;
        }

        /**
         * @tparam U 
         */
        template <typename U>
        WeakPtr(const U& object)
            requires(IsBaseOf<T, U>)
            : m_link(object.template make_weak_ptr<U>().take_link())
        {
        }

        /**
         * @tparam U 
         */
        template <typename U>
        WeakPtr(const U* object)
            requires(IsBaseOf<T, U>)
        {
            if(object)
                m_link = object->template make_weak_ptr<U>().take_link();
        }

        /**
         * @tparam U 
         */
        template <typename U>
        WeakPtr(RefPtr<U> const& object)
            requires(IsBaseOf<T, U>)
        {
            if(object)
                m_link = object->template make_weak_ptr<U>().take_link();
        }

        /**
         * @tparam U 
         */
        template <typename U>
        WeakPtr(NonnullRefPtr<U> const& object)
            requires(IsBaseOf<T, U>)
        {
            m_link = object->template make_weak_ptr<U>().take_link();
        }

        /**
         * @tparam U 
         */
        template <typename U>
        WeakPtr& operator=(const U& object)
            requires(IsBaseOf<T, U>)
        {
            m_link = object.template make_weak_ptr<U>().take_link();
            return *this;
        }

        /**
         * @tparam U 
         */
        template <typename U>
        WeakPtr& operator=(const U* object)
            requires(IsBaseOf<T, U>)
        {
            if(object)
                m_link = object->template make_weak_ptr<U>().take_link();
            else
                m_link = nullptr;
            return *this;
        }

        /**
         * @tparam U 
         */
        template <typename U>
        WeakPtr& operator=(RefPtr<U> const& object)
            requires(IsBaseOf<T, U>)
        {
            if(object)
                m_link = object->template make_weak_ptr<U>().take_link();
            else
                m_link = nullptr;
            return *this;
        }

        /**
         * @tparam U 
         */
        template <typename U>
        WeakPtr& operator=(NonnullRefPtr<U> const& object)
            requires(IsBaseOf<T, U>)
        {
            m_link = object->template make_weak_ptr<U>().take_link();
            return *this;
        }

        /**
         * @return RefPtr<T> 
         */
        [[nodiscard]] RefPtr<T> strong_ref() const
        {
            return RefPtr<T>{ptr()};
        }

        /**
         * @return T* 
         */
        T* ptr() const
        {
            return unsafe_ptr();
        }

        /**
         * @return T* 
         */
        T* operator->()
        {
            return unsafe_ptr();
        }

        /**
         * @return const T* 
         */
        const T* operator->() const
        {
            return unsafe_ptr();
        }

        /**
         * @return const T* 
         */
        operator const T*() const
        {
            return unsafe_ptr();
        }

        /**
         * @return T* 
         */
        operator T*()
        {
            return unsafe_ptr();
        }

        /**
         * @return T* 
         */
        [[nodiscard]] T* unsafe_ptr() const
        {
            if(m_link)
                return m_link->template unsafe_ptr<T>();
            return nullptr;
        }

        /**
         * @return true 
         * @return false 
         */
        operator bool() const
        {
            return m_link ? !m_link->is_null() : false;
        }

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool is_null() const
        {
            return !m_link || m_link->is_null();
        }

        void clear()
        {
            m_link = nullptr;
        }

        /**
         * @return RefPtr<WeakLink> 
         */
        [[nodiscard]] RefPtr<WeakLink> take_link()
        {
            return move(m_link);
        }

    private:
        /**
         * @brief Construct a new WeakPtr object
         * 
         * @param link 
         */
        WeakPtr(RefPtr<WeakLink> const& link)
            : m_link(link)
        {
        }

        RefPtr<WeakLink> m_link;
    }; // class [[nodiscard]] WeakPtr

    /**
     * @tparam T 
     * @tparam U 
     * @return ErrorOr<WeakPtr<U>> 
     */
    template <typename T>
    template <typename U>
    inline ErrorOr<WeakPtr<U>> Weakable<T>::try_make_weak_ptr() const
    {
        if(!m_link)
            m_link = TRY(adopt_nonnull_ref_or_enomem(new(nothrow) WeakLink(const_cast<T&>(static_cast<T const&>(*this)))));

        return WeakPtr<U>(m_link);
    }

    /**
     * @tparam T 
     */
    template <typename T>
    struct Formatter<WeakPtr<T>> : Formatter<const T*>
    {
        /**
         * @param builder 
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> format(FormatBuilder& builder, WeakPtr<T> const& value)
        {
            return Formatter<const T*>::format(builder, value.ptr());
        }
    };

    /**
     * @tparam T 
     * @param ptr 
     * @return ErrorOr<WeakPtr<T>> 
     */
    template <typename T>
    ErrorOr<WeakPtr<T>> try_make_weak_ptr_if_nonnull(T const* ptr)
    {
        if(ptr)
        {
            return ptr->template try_make_weak_ptr<T>();
        }
        return WeakPtr<T>{};
    }

    /**
     * @tparam T 
     * @param ptr 
     * @return WeakPtr<T> 
     */
    template <typename T>
    WeakPtr<T> make_weak_ptr_if_nonnull(T const* ptr)
    {
        return MUST(try_make_weak_ptr_if_nonnull(ptr));
    }

} // namespace Mods

using Mods::WeakPtr;
#endif
