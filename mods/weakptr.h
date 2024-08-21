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

#include <mods/logstream.h>
#include <mods/weakable.h>

namespace Mods
{   
    
    /**
     * @tparam T 
     */
    template<typename T>
    class WeakPtr 
    {
        template<typename U>
        friend class Weakable;

    public:
        WeakPtr() { }

        /// @brief Construct a new Weak Ptr object
        WeakPtr(std::nullptr_t) { }

        /**
         * @tparam U 
         * @tparam EnableIf<IsBaseOf<T, U>::value>::Type 
         * @param other 
         */
        template<typename U, typename EnableIf<IsBaseOf<T, U>::value>::Type* = nullptr>
        WeakPtr(const WeakPtr<U>& other)
            : m_link(other.m_link)
        {
        }

        /**
         * @tparam U 
         * @tparam EnableIf<IsBaseOf<T, U>::value>::Type 
         * @param other 
         */
        template<typename U, typename EnableIf<IsBaseOf<T, U>::value>::Type* = nullptr>
        WeakPtr(WeakPtr<U>&& other)
            : m_link(other.take_link())
        {
        }

        /**
         * @tparam U 
         * @tparam EnableIf<IsBaseOf<T, U>::value>::Type 
         * @param other 
         * @return WeakPtr& 
         */
        template<typename U, typename EnableIf<IsBaseOf<T, U>::value>::Type* = nullptr>
        WeakPtr& operator=(WeakPtr<U>&& other)
        {
            m_link = other.take_link();
            return *this;
        }

        /**
         * @tparam U 
         * @tparam EnableIf<IsBaseOf<T, U>::value>::Type 
         * @param other 
         * @return WeakPtr& 
         */
        template<typename U, typename EnableIf<IsBaseOf<T, U>::value>::Type* = nullptr>
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
         * @tparam U 
         * @tparam EnableIf<IsBaseOf<T, U>::value>::Type 
         * @param object 
         */
        template<typename U, typename EnableIf<IsBaseOf<T, U>::value>::Type* = nullptr>
        WeakPtr(const U& object)
            : m_link(object.template make_weak_ptr<U>().take_link())
        {
        }

        /**
         * @tparam U 
         * @tparam EnableIf<IsBaseOf<T, U>::value>::Type 
         * @param object 
         */
        template<typename U, typename EnableIf<IsBaseOf<T, U>::value>::Type* = nullptr>
        WeakPtr(const U* object)
        {
            if (object)
                m_link = object->template make_weak_ptr<U>().take_link();
        }

        /**
         * @tparam U 
         * @tparam EnableIf<IsBaseOf<T, U>::value>::Type 
         * @param object 
         */
        template<typename U, typename EnableIf<IsBaseOf<T, U>::value>::Type* = nullptr>
        WeakPtr(const RefPtr<U>& object)
        {
            object.do_while_locked([&](U* obj) {
                if (obj)
                    obj->template make_weak_ptr<U>().take_link();
            });
        }

        /**
         * @tparam U 
         * @tparam EnableIf<IsBaseOf<T, U>::value>::Type 
         * @param object 
         */
        template<typename U, typename EnableIf<IsBaseOf<T, U>::value>::Type* = nullptr>
        WeakPtr(const NonnullRefPtr<U>& object)
        {
            object.do_while_locked([&](U* obj) {
                if (obj)
                    obj->template make_weak_ptr<U>().take_link();
            });
        }

        /**
         * @tparam U 
         * @tparam EnableIf<IsBaseOf<T, U>::value>::Type 
         * @param object 
         * @return WeakPtr& 
         */
        template<typename U, typename EnableIf<IsBaseOf<T, U>::value>::Type* = nullptr>
        WeakPtr& operator=(const U& object)
        {
            m_link = object.template make_weak_ptr<U>().take_link();
            return *this;
        }

        /**
         * @tparam U 
         * @tparam EnableIf<IsBaseOf<T, U>::value>::Type 
         * @param object 
         * @return WeakPtr& 
         */
        template<typename U, typename EnableIf<IsBaseOf<T, U>::value>::Type* = nullptr>
        WeakPtr& operator=(const U* object)
        {
            if (object)
                m_link = object->template make_weak_ptr<U>().take_link();
            else
                m_link = nullptr;
            return *this;
        }   

        /**
         * @tparam U 
         * @tparam EnableIf<IsBaseOf<T, U>::value>::Type 
         * @param object 
         * @return WeakPtr& 
         */
        template<typename U, typename EnableIf<IsBaseOf<T, U>::value>::Type* = nullptr>
        WeakPtr& operator=(const RefPtr<U>& object)
        {
            object.do_while_locked([&](U* obj) {
                if (obj)
                    m_link = obj->template make_weak_ptr<U>().take_link();
                else
                    m_link = nullptr;
            });
            return *this;
        }

        /**
         * @tparam U 
         * @tparam EnableIf<IsBaseOf<T, U>::value>::Type 
         * @param object 
         * @return WeakPtr& 
         */
        template<typename U, typename EnableIf<IsBaseOf<T, U>::value>::Type* = nullptr>
        WeakPtr& operator=(const NonnullRefPtr<U>& object)
        {
            object.do_while_locked([&](U* obj) {
                if (obj)
                    m_link = obj->template make_weak_ptr<U>().take_link();
                else
                    m_link = nullptr;
            });
            return *this;
        }

        /**
         * @return RefPtr<T> 
         */
        RefPtr<T> strong_ref() const
        {
            RefPtr<T> ref;
            m_link.do_while_locked([&](WeakLink* link) {
                if (link)
                    ref = link->template strong_ref<T>();
            });
            return ref;
        }

    #ifndef KERNEL
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
    #endif

        /**
         * @return T* 
         */
        T* unsafe_ptr() const
        {
            T* ptr = nullptr;
            m_link.do_while_locked([&](WeakLink* link) {
                if (link)
                    ptr = link->unsafe_ptr<T>();
            });
            return ptr;
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
        bool is_null() const 
        { 
            return !m_link || m_link->is_null(); 
        }

        /// @brief: clear
        void clear() 
        { 
            m_link = nullptr; 
        }

        /**
         * @return RefPtr<WeakLink> 
         */
        RefPtr<WeakLink> take_link() 
        { 
            return move(m_link); 
        }

    private:
        /**
         * @param link 
         */
        WeakPtr(const RefPtr<WeakLink>& link)
            : m_link(link)
        {
        }

        RefPtr<WeakLink> m_link;
    };

    /**
     * @tparam T 
     * @tparam U 
     * @return WeakPtr<U> 
     */
    template<typename T>
    template<typename U>
    inline WeakPtr<U> Weakable<T>::make_weak_ptr() const
    {
        #ifdef DEBUG
            ASSERT(!m_being_destroyed);
        #endif
            if (!m_link) {
                m_link.assign_if_null(adopt(*new WeakLink(const_cast<T&>(static_cast<const T&>(*this)))));
            }
            return WeakPtr<U>(m_link);
    }

    /**
     * @tparam T 
     * @param stream 
     * @param value 
     * @return const LogStream& 
     */
    template<typename T>
    inline const LogStream& operator<<(const LogStream& stream, const WeakPtr<T>& value)
    {
        #ifdef KERNEL
            auto ref = value.strong_ref();
            return stream << ref.ptr();
        #else
            return stream << value.ptr();
        #endif
    }

    /**
     * @tparam T 
     */
    template<typename T>
    struct Formatter<WeakPtr<T>> : Formatter<const T*> {
        void format(TypeErasedFormatParams& params, FormatBuilder& builder, const WeakPtr<T>& value)
        {
    #ifdef KERNEL
            auto ref = value.strong_ref();
            Formatter<const T*>::format(params, builder, ref.ptr());
    #else
            Formatter<const T*>::format(params, builder, value.ptr());
    #endif
        }
    };

}

using Mods::WeakPtr;
