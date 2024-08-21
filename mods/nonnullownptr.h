/**
 * @file nonnullownptr.h
 * @author Krisna Pranav
 * @brief nonnullownptr.h
 * @version 6.0
 * @date 2023-07-30
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include "assertions.h"
#include "logstream.h"
#include "refcounted.h"
#include "stdlibextra.h"
#include "types.h"
#include "traits.h"

namespace Mods 
{

    template<typename T, typename PtrTraits>
    class RefPtr;

    /**
     * @tparam T 
     */
    template<typename T>
    class NonnullRefPtr;
    
    template<typename T>
    class WeakPtr;

    template<typename T>
    class NonnullOwnPtr 
    {
    public:
        using ElementType = T;

        enum AdoptTag { Adopt };

        /**
         * @param ptr 
         */
        NonnullOwnPtr(AdoptTag, T& ptr)
            : m_ptr(&ptr)
        {
            static_assert(
                requires { requires typename T::AllowOwnPtr()(); } || !requires(T obj) { requires !typename T::AllowOwnPtr()(); obj.ref(); obj.unref(); },
                "Use NonnullRefPtr<> for RefCounted types");
        }

        /**
         * @param other 
         */
        NonnullOwnPtr(NonnullOwnPtr&& other)
            : m_ptr(other.leak_ptr())
        {
            ASSERT(m_ptr);
        }

        /**
         * @tparam U 
         * @param other 
         */
        template<typename U>
        NonnullOwnPtr(NonnullOwnPtr<U>&& other)
            : m_ptr(other.leak_ptr())
        {
            ASSERT(m_ptr);
        }

        ~NonnullOwnPtr()
        {
            clear();
    #ifdef SANITIZE_PTRS
            if constexpr (sizeof(T*) == 8)
                m_ptr = (T*)(0xe3e3e3e3e3e3e3e3);
            else
                m_ptr = (T*)(0xe3e3e3e3);
    #endif
        }

        NonnullOwnPtr(const NonnullOwnPtr&) = delete;

        /**
         * @tparam U 
         */
        template<typename U>
        NonnullOwnPtr(const NonnullOwnPtr<U>&) = delete;
        NonnullOwnPtr& operator=(const NonnullOwnPtr&) = delete;

        /**
         * @tparam U 
         * @return NonnullOwnPtr& 
         */
        template<typename U>
        NonnullOwnPtr& operator=(const NonnullOwnPtr<U>&) = delete;

        /**
         * @tparam U 
         * @tparam PtrTraits 
         */
        template<typename U, typename PtrTraits = RefPtrTraits<U>>
        NonnullOwnPtr(const RefPtr<U, PtrTraits>&) = delete;

        /**
         * @tparam U 
         */
        template<typename U>
        NonnullOwnPtr(const NonnullRefPtr<U>&) = delete;

        /**
         * @tparam U 
         */
        template<typename U>
        NonnullOwnPtr(const WeakPtr<U>&) = delete;

        /**
         * @tparam U 
         * @tparam PtrTraits 
         * @return NonnullOwnPtr& 
         */
        template<typename U, typename PtrTraits = RefPtrTraits<U>>
        NonnullOwnPtr& operator=(const RefPtr<U, PtrTraits>&) = delete;

        /**
         * @tparam U 
         * @return NonnullOwnPtr& 
         */
        template<typename U>
        NonnullOwnPtr& operator=(const NonnullRefPtr<U>&) = delete;

        /**
         * @tparam U 
         * @return NonnullOwnPtr& 
         */
        template<typename U>
        NonnullOwnPtr& operator=(const WeakPtr<U>&) = delete;

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
        template<typename U>
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
            return m_ptr; 
        }

        /**
         * @return const T* 
         */
        const T* operator->() const 
        { 
            return m_ptr; 
        }

        /**
         * @return T& 
         */
        T& operator*() 
        { 
            return *m_ptr; 
        }

        /**
         * @return const T& 
         */
        const T& operator*() const 
        { 
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
        operator bool() const = delete;

        /**
         * @return true 
         * @return false 
         */
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
        template<typename U>
        void swap(NonnullOwnPtr<U>& other)
        {
            ::swap(m_ptr, other.m_ptr);
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

    private:
        /// @brief: clear[m_ptr]
        void clear()
        {
            if (!m_ptr)
                return;
            delete m_ptr;
            m_ptr = nullptr;
        }

        T* m_ptr = nullptr;
    }; // class NonnullOwnPtr

    /**
     * @tparam T 
     * @param object 
     * @return NonnullOwnPtr<T> 
     */
    template<typename T>
    inline NonnullOwnPtr<T> adopt_own(T& object)
    {
        return NonnullOwnPtr<T>(NonnullOwnPtr<T>::Adopt, object);
    } // inline adopt_own

    /**
     * @tparam T 
     * @tparam Args 
     * @param args 
     * @return NonnullOwnPtr<T> 
     */
    template<class T, class... Args>
    inline NonnullOwnPtr<T>
    make(Args&&... args)
    {
        return NonnullOwnPtr<T>(NonnullOwnPtr<T>::Adopt, *new T(forward<Args>(args)...));
    } // inline make

    /**
     * @tparam T 
     */
    template<typename T>
    struct Traits<NonnullOwnPtr<T>> : public GenericTraits<NonnullOwnPtr<T>> 
    {
        using PeekType = const T*;
        
        /**
         * @param p 
         * @return unsigned 
         */
        static unsigned hash(const NonnullOwnPtr<T>& p) 
        { 
            return int_hash((u32)p.ptr()); 
        }

        /**
         * @param a 
         * @param b 
         * @return true 
         * @return false 
         */
        static bool equals(const NonnullOwnPtr<T>& a, const NonnullOwnPtr<T>& b) 
        { 
            return a.ptr() == b.ptr(); 
        }
    }; // struct Traits

    /**
     * @tparam T 
     * @param stream 
     * @param value 
     * @return const LogStream& 
     */
    template<typename T>
    inline const LogStream& operator<<(const LogStream& stream, const NonnullOwnPtr<T>& value)
    {
        return stream << value.ptr();
    } // const LogStream
    
    /**
     * @tparam T 
     * @tparam U 
     * @param a 
     * @param b 
     */
    template<typename T, typename U>
    inline void swap(NonnullOwnPtr<T>& a, NonnullOwnPtr<U>& b)
    {
        a.swap(b);
    } // void swap

    /**
     * @tparam T 
     */
    template<typename T>
    struct Formatter<NonnullOwnPtr<T>> : Formatter<const T*> 
    {
        /**
         * @param params 
         * @param builder 
         * @param value 
         */
        void format(TypeErasedFormatParams& params, FormatBuilder& builder, const NonnullOwnPtr<T>& value)
        {
            Formatter<const T*>::format(params, builder, value.ptr());
        }
    }; // struct Formatter

}

using Mods::adopt_own;
using Mods::make;
using Mods::NonnullOwnPtr;