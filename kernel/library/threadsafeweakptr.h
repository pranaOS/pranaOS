/**
 * @file threadsafeweakptr.h
 * @author Krisna Pranav
 * @brief Thread Safe Weak Ptr
 * @version 0.1
 * @date 2024-10-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <mods/weakable.h>

namespace Mods
{

    template<typename T>
    class [[nodiscard]] WeakPtr 
    {
        template<typename U>
        friend class Weakable;

    public:
        /**
         * @brief Construct a new Weak Ptr object
         * 
         */
        WeakPtr() = default;

        /**
         * @brief Construct a new Weak Ptr object
         * 
         * @tparam U 
         * @tparam EnableIf<IsBaseOf<T, U>>::Type 
         * @param other 
         */
        template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
        WeakPtr(const WeakPtr<U>& other)
            : m_link(other.m_link)
        {
        }

        /**
         * @tparam U 
         * @tparam EnableIf<IsBaseOf<T, U>>::Type 
         * @param other 
         */
        template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
        WeakPtr(WeakPtr<U>&& other)
            : m_link(other.take_link())
        {
        }

        /**
         * @tparam U 
         * @tparam EnableIf<IsBaseOf<T, U>>::Type 
         * @param other 
         * @return WeakPtr& 
         */
        template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
        WeakPtr& operator=(WeakPtr<U>&& other)
        {
            m_link = other.take_link();
            return *this;
        }

        /**
         * @tparam U 
         * @tparam EnableIf<IsBaseOf<T, U>>::Type 
         * @param other 
         * @return WeakPtr& 
         */
        template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
        WeakPtr& operator=(const WeakPtr<U>& other)
        {
            if ((const void*)this != (const void*)&other)
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
         * @brief Construct a new Weak Ptr object
         * 
         * @tparam U 
         * @tparam EnableIf<IsBaseOf<T, U>>::Type 
         * @param object 
         */
        template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
        WeakPtr(const U& object)
            : m_link(object.template try_make_weak_ptr<U>().release_value_but_fixme_should_propagate_errors().take_link())
        {
        }

        /**
         * @brief Construct a new Weak Ptr object
         * 
         * @tparam U 
         * @tparam EnableIf<IsBaseOf<T, U>>::Type 
         * @param object 
         */
        template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
        WeakPtr(const U* object)
        {
            if (object)
                m_link = object->template try_make_weak_ptr<U>().release_value_but_fixme_should_propagate_errors().take_link();
        }

        /**
         * @brief Construct a new Weak Ptr object
         * 
         * @tparam U 
         * @tparam EnableIf<IsBaseOf<T, U>>::Type 
         * @param object 
         */
        template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
        WeakPtr(const RefPtr<U>& object)
        {
            object.do_while_locked([&](U* obj) {
                if (obj)
                    m_link = obj->template try_make_weak_ptr<U>().release_value_but_fixme_should_propagate_errors().take_link();
            });
        }

        /**
         * @brief Construct a new Weak Ptr object
         * 
         * @tparam U 
         * @tparam EnableIf<IsBaseOf<T, U>>::Type 
         * @param object 
         */
        template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
        WeakPtr(const NonnullRefPtr<U>& object)
        {
            object.do_while_locked([&](U* obj) {
                if (obj)
                    m_link = obj->template try_make_weak_ptr<U>().release_value_but_fixme_should_propagate_errors().take_link();
            });
        }

        /**
         * @tparam U 
         * @tparam EnableIf<IsBaseOf<T, U>>::Type 
         * @param object 
         * @return WeakPtr& 
         */
        template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
        WeakPtr& operator=(const U& object)
        {
            m_link = object.template try_make_weak_ptr<U>().release_value_but_fixme_should_propagate_errors().take_link();
            return *this;
        }

        /**
         * @tparam U 
         * @tparam EnableIf<IsBaseOf<T, U>>::Type 
         * @param object 
         * @return WeakPtr& 
         */
        template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
        WeakPtr& operator=(const U* object)
        {
            if (object)
                m_link = object->template try_make_weak_ptr<U>().release_value_but_fixme_should_propagate_errors().take_link();
            else
                m_link = nullptr;
            return *this;
        }

        /**
         * @tparam U 
         * @tparam EnableIf<IsBaseOf<T, U>>::Type 
         * @param object 
         * @return WeakPtr& 
         */
        template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
        WeakPtr& operator=(const RefPtr<U>& object)
        {
            object.do_while_locked([&](U* obj) {
                if (obj)
                    m_link = obj->template try_make_weak_ptr<U>().release_value_but_fixme_should_propagate_errors().take_link();
                else
                    m_link = nullptr;
            });
            return *this;
        }

        /**
         * @tparam U 
         * @tparam EnableIf<IsBaseOf<T, U>>::Type 
         * @param object 
         * @return WeakPtr& 
         */
        template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
        WeakPtr& operator=(const NonnullRefPtr<U>& object)
        {
            object.do_while_locked([&](U* obj) {
                if (obj)
                    m_link = obj->template try_make_weak_ptr<U>().release_value_but_fixme_should_propagate_errors().take_link();
                else
                    m_link = nullptr;
            });
            return *this;
        }

        /**
         * @return RefPtr<T> 
         */
        [[nodiscard]] RefPtr<T> strong_ref() const
        {
            RefPtr<T> ref;
            m_link.do_while_locked([&](WeakLink* link) {
                if (link)
                    ref = link->template strong_ref<T>();
            });
            return ref;
        }

        /**
         * @return T* 
         */
        [[nodiscard]] T* unsafe_ptr() const
        {
            T* ptr = nullptr;
            m_link.do_while_locked([&](WeakLink* link) {
                if (link)
                    ptr = link->unsafe_ptr<T>();
            });
            return ptr;
        }

        operator bool() const 
        { 
            return m_link ? !m_link->is_null() : false; 
        }

        [[nodiscard]] bool is_null() const 
        { 
            return !m_link || m_link->is_null(); 
        }

        void clear() 
        { 
            m_link = nullptr; 
        }

        [[nodiscard]] RefPtr<WeakLink> take_link() 
        { 
            return move(m_link); 
        }

    private:
        /**
         * @brief Construct a new Weak Ptr object
         * 
         * @param link 
         */
        WeakPtr(const RefPtr<WeakLink>& link)
            : m_link(link)
        {
        }

        RefPtr<WeakLink> m_link;
    }; // class WeakPtr

    /**
     * @tparam T 
     * @tparam U 
     * @return ErrorOr<WeakPtr<U>> 
     */
    template<typename T>
    template<typename U>
    inline ErrorOr<WeakPtr<U>> Weakable<T>::try_make_weak_ptr() const
    {
        if constexpr (IsBaseOf<RefCountedBase, T>) {
            if (!static_cast<const T*>(this)->try_ref())
                return WeakPtr<U> {};
        } else {
            if (m_being_destroyed.load(AK::MemoryOrder::memory_order_acquire))
                return WeakPtr<U> {};
        }
        if (!m_link) {
            m_link.assign_if_null(TRY(adopt_nonnull_ref_or_enomem(new (nothrow) WeakLink(const_cast<T&>(static_cast<const T&>(*this))))));
        }

        WeakPtr<U> weak_ptr(m_link);

        if constexpr (IsBaseOf<RefCountedBase, T>) {
            if (static_cast<const T*>(this)->unref()) {
                VERIFY(!weak_ptr.strong_ref());
                return WeakPtr<U> {};
            }
        }
        return weak_ptr;
    }

    /**
     * @tparam T 
     */
    template<typename T>
    struct Formatter<WeakPtr<T>> : Formatter<const T*> {
        ErrorOr<void> format(FormatBuilder& builder, WeakPtr<T> const& value)
        {
            auto ref = value.strong_ref();
            return Formatter<const T*>::format(builder, ref.ptr());
        }
    };

    /**
     * @tparam T 
     * @param ptr 
     * @return ErrorOr<WeakPtr<T>> 
     */
    template<typename T>
    ErrorOr<WeakPtr<T>> try_make_weak_ptr_if_nonnull(T const* ptr)
    {
        if (ptr) {
            return ptr->template try_make_weak_ptr<T>();
        }
        return WeakPtr<T> {};
    }

} // namespace Mods

using Mods::WeakPtr;
