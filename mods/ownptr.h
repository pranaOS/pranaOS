/**
 * @file ownptr.h
 * @author Krisna Pranav
 * @brief ownptr
 * @version 1.0
 * @date 2023-07-10
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "nonnull_refptr.h"
#include "refcounted.h"

namespace Mods 
{

    template<typename T>
    class OwnPtr 
    {
    public:

        /**
         * @brief Construct a new Own Ptr object
         * 
         */
        OwnPtr() { }

        /**
         * @param ptr 
         */
        explicit OwnPtr(T* ptr) : m_ptr(ptr) {
            static_assert(
                requires { requires typename T::AllowOwnPtr()(); } || !requires(T obj) { requires !typename T::AllowOwnPtr()(); obj.ref(); obj.unref(); },
                "Use RefPtr<> for RefCounted types");
        }

        /**
         * @param other 
         */
        OwnPtr(OwnPtr&& other) : m_ptr(other.leak_ptr())
        {
        }

        /**
         * @tparam U 
         * @param other 
         */
        template<typename U>
        OwnPtr(NonnullOwnPtr<U>&& other) : m_ptr(other.leak_ptr())
        {
        }

        /**
         * @tparam U 
         * @param other 
         */
        template<typename U>
        OwnPtr(OwnPtr<U>&& other) : m_ptr(other.leak_ptr())
        {
        }


        OwnPtr(std::nullptr_t) {};

        ~OwnPtr()
        {
            clear();
    #ifdef SANITIZE_PTRS
            if constexpr (sizeof(T*) == 8)
                m_ptr = (T*)(0xe1e1e1e1e1e1e1e1);
            else
                m_ptr = (T*)(0xe1e1e1e1);
    #endif
        }

        OwnPtr(const OwnPtr&) = delete;
        template<typename U>
        OwnPtr(const OwnPtr<U>&) = delete;
        OwnPtr& operator=(const OwnPtr&) = delete;
        template<typename U>
        OwnPtr& operator=(const OwnPtr<U>&) = delete;

        template<typename U>
        OwnPtr(const NonnullOwnPtr<U>&) = delete;
        template<typename U>
        OwnPtr& operator=(const NonnullOwnPtr<U>&) = delete;
        template<typename U>
        OwnPtr(const RefPtr<U>&) = delete;
        template<typename U>
        OwnPtr(const NonnullRefPtr<U>&) = delete;
        template<typename U>
        OwnPtr(const WeakPtr<U>&) = delete;
        template<typename U>
        OwnPtr& operator=(const RefPtr<U>&) = delete;
        template<typename U>
        OwnPtr& operator=(const NonnullRefPtr<U>&) = delete;
        template<typename U>
        OwnPtr& operator=(const WeakPtr<U>&) = delete;

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
        template<typename U>
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
        template<typename U>
        OwnPtr& operator=(NonnullOwnPtr<U>&& other)
        {
            OwnPtr ptr(move(other));
            swap(ptr);
            ASSERT(m_ptr);
            return *this;
        }

        /**
         * @param ptr 
         * @return OwnPtr& 
         */
        OwnPtr& operator=(T* ptr)
        {
            if (m_ptr != ptr)
                delete m_ptr;
            m_ptr = ptr;
            return *this;
        }

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
            ASSERT(m_ptr);
            return NonnullOwnPtr<T>(NonnullOwnPtr<T>::Adopt, *leak_ptr());
        }

        /**
         * @tparam U 
         * @return NonnullOwnPtr<U> 
         */
        template<typename U>
        NonnullOwnPtr<U> release_nonnull()
        {
            ASSERT(m_ptr);
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
            ASSERT(m_ptr);
            return m_ptr;
        }

        /**
         * @return const T* 
         */
        const T* operator->() const
        {
            ASSERT(m_ptr);
            return m_ptr;
        }

        /**
         * @return T& 
         */
        T& operator*()
        {
            ASSERT(m_ptr);
            return *m_ptr;
        }

        /**
         * @return const T& 
         */
        const T& operator*() const
        {
            ASSERT(m_ptr);
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
        template<typename U>
        void swap(OwnPtr<U>& other)
        {
            ::swap(m_ptr, other.m_ptr);
        }

    private:
        T* m_ptr = nullptr;
    };

    /**
     * @tparam T 
     * @tparam U 
     * @param a 
     * @param b 
     */
    template<typename T, typename U>
    inline void swap(OwnPtr<T>& a, OwnPtr<U>& b)
    {
        a.swap(b);
    }

    /**
     * @tparam T 
     */
    template<typename T>
    struct Traits<OwnPtr<T>> : public GenericTraits<OwnPtr<T>> {
        using PeekType = const T*;
        static unsigned hash(const OwnPtr<T>& p) { return ptr_hash(p.ptr()); }
        static bool equals(const OwnPtr<T>& a, const OwnPtr<T>& b) { return a.ptr() == b.ptr(); }
    };
    
    /**
     * @tparam T 
     * @param stream 
     * @param value 
     * @return const LogStream& 
     */
    template<typename T>
    inline const LogStream& operator<<(const LogStream& stream, const OwnPtr<T>& value)
    {
        return stream << value.ptr();
    }

} // namespace Mods

using Mods::OwnPtr;
