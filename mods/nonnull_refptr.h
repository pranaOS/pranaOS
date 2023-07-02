/**
 * @file nonnull_refptr.h
 * @author Krisna Pranav
 * @brief nonnullrefptr
 * @version 1.0
 * @date 2023-07-01
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "assertions.h"
#include "atomic.h"
#include "logstream.h"
#include "stdlibextra.h"
#include "types.h"
#include "format.h"
#ifdef KERNEL
#   include <kernel/arch/i386/cpu.h>
#endif

namespace Mods {

    /**
     * @tparam T 
     */
    template<typename T>
    class OwnPtr;
    
    /**
     * @tparam T 
     * @tparam PtrTraits 
     */
    template<typename T, typename PtrTraits>
    class RefPtr;

    /**
     * @tparam T 
     * @param ptr 
     * @return ALWAYS_INLINE 
     */
    template<typename T>
    ALWAYS_INLINE void ref_if_not_null(T* ptr) {
        if (ptr)
            ptr->ref();
    }

    /**
     * @tparam T 
     * @param ptr 
     * @return ALWAYS_INLINE 
     */
    template<typename T>
    ALWAYS_INLINE void unref_if_not_null(T* ptr) {
        if (ptr)
            ptr->unref();
    }

    template<typename T>
    class NonnullRefPtr {

        /**
         * @tparam U 
         * @tparam P 
         */
        template<typename U, typename P>
        friend class RefPtr;

        /**
         * @tparam U 
         */
        template<typename U>
        friend class NonnullRefPtr;

        /**
         * @tparam U 
         */
        template<typename U>
        friend class WeakPtr;

    public:
        using ElementType = T;

        enum AdoptTag { Adopt };

        /**
         * @param object 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE NonnullRefPtr(const T& object) : m_bits((FlatPtr)&object) {
            ASSERT(!(m_bits & 1));
            const_cast<T&>(object).ref();
        }

        /**
         * @tparam U 
         * @param object 
         * @return ALWAYS_INLINE 
         */
        template<typename U>
        ALWAYS_INLINE NonnullRefPtr(const U& object) : m_bits((FlatPtr) static_cast<const T*>(&object))
        {
            ASSERT(!(m_bits & 1));
            const_cast<T&>(static_cast<const T&>(object)).ref();
        }

        /**
         * @param object 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE NonnullRefPtr(AdoptTag, T& object) : m_bits((FlatPtr)&object)
        {
            ASSERT(!(m_bits & 1));
        }

        /**
         * @param other 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE NonnullRefPtr(NonnullRefPtr&& other) : m_bits((FlatPtr)&other.leak_ref())
        {
            ASSERT(!(m_bits & 1));
        }

        /**
         * @tparam U 
         * @param other 
         * @return ALWAYS_INLINE 
         */
        template<typename U>
        ALWAYS_INLINE NonnullRefPtr(NonnullRefPtr<U>&& other) : m_bits((FlatPtr)&other.leak_ref())
        {
            ASSERT(!(m_bits & 1));
        }

        /** 
         * @param other 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE NonnullRefPtr(const NonnullRefPtr& other) : m_bits((FlatPtr)other.add_ref())
        {
            ASSERT(!(m_bits & 1));
        }

        /**
         * @tparam U 
         * @param other 
         * @return ALWAYS_INLINE 
         */
        template<typename U>
        ALWAYS_INLINE NonnullRefPtr(const NonnullRefPtr<U>& other) : m_bits((FlatPtr)other.add_ref())
        {
            ASSERT(!(m_bits & 1));
        }  
        
        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE ~NonnullRefPtr() {
            assign(nullptr);
    #ifdef SANITIZE_PTRS
            if constexpr (sizeof(T*) == 8)
                m_bits.store(0xb0b0b0b0b0b0b0b0, Mods::MemoryOrder::memory_order_relaxed);
            else
                m_bits.store(0xb0b0b0b0, Mods::MemoryOrder::memory_order_relaxed);
    #endif
        }

        /**
         * @tparam U 
         */
        template<typename U>
        NonnullRefPtr(const OwnPtr<U>&) = delete;

        /**
         * @tparam U 
         * @return NonnullRefPtr& 
         */
        template<typename U>
        NonnullRefPtr& operator=(const OwnPtr<U>&) = delete;

        /**
         * @tparam U 
         */
        template<typename U>
        NonnullRefPtr(const RefPtr<U>&) = delete;

        /**
         * @tparam U 
         * @return NonnullRefPtr& 
         */
        template<typename U>
        NonnullRefPtr& operator=(const RefPtr<U>&) = delete;

        /**
         * @brief Construct a new Nonnull Ref Ptr object
         * 
         */
        NonnullRefPtr(const RefPtr<T>&) = delete;

        /**
         * @return NonnullRefPtr& 
         */
        NonnullRefPtr& operator=(const RefPtr<T>&) = delete;

        /**
         * @param other 
         * @return NonnullRefPtr& 
         */
        NonnullRefPtr& operator=(const NonnullRefPtr& other) {
            if (this != &other)
                assign(other.add_ref());
            return *this;
        }

        /**
         * @tparam U 
         * @param other 
         * @return NonnullRefPtr& 
         */
        template<typename U>    
        NonnullRefPtr& operator=(const NonnullRefPtr<U>& other) {
            assign(other.add_ref());
            return *this;
        }

        /**
         * @param other 
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE NonnullRefPtr& operator=(NonnullRefPtr&& other) {
            if (this != &other)
                assign(&other.leak_ref());
            return *this;
        }

        /**
         * @tparam U 
         * @param other 
         * @return NonnullRefPtr& 
         */
        template<typename U>
        NonnullRefPtr& operator=(NonnullRefPtr<U>&& other) {
            assign(&other.leak_ref());
            return *this;
        }

        /**
         * @param object 
         * @return NonnullRefPtr& 
         */
        NonnullRefPtr& operator=(const T& object) {
            const_cast<T&>(object).ref();
            assign(const_cast<T*>(&object));
            return *this;
        }

        /**
         * @return ALWAYS_INLINE& 
         */
        [[nodiscard]] ALWAYS_INLINE T& leak_ref() {
            T* ptr = exchange(nullptr);
            ASSERT(ptr);
            return *ptr;
        }

        /**
         * @return ALWAYS_INLINE* 
         */
        ALWAYS_INLINE T* ptr() {
            return as_nonnull_ptr();
        }

        /**
         * @return ALWAYS_INLINE const* 
         */
        ALWAYS_INLINE const T* ptr() const {
            return as_nonnull_ptr();
        }

        /**
         * @return ALWAYS_INLINE* 
         */
        ALWAYS_INLINE T* operator->() {
            return as_nonnull_ptr();
        }

        /**
         * @return ALWAYS_INLINE const* 
         */
        ALWAYS_INLINE const T* operator->() const {
            return as_nonnull_ptr();
        }

        /**
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE T& operator*() {
            return *as_nonnull_ptr();
        }   

        /**
         * @return ALWAYS_INLINE const& 
         */
        ALWAYS_INLINE const T& operator*() const {
            return *as_nonnull_ptr();
        }

        /**
         * @return T* 
         */
        ALWAYS_INLINE operator T*() {
            return as_nonnull_ptr();
        }  
        
        /**
         * @return const T* 
         */
        ALWAYS_INLINE operator const T*() const {
            return as_nonnull_ptr();
        }

        /**
         * @return T& 
         */
        ALWAYS_INLINE operator T&() {
            return *as_nonnull_ptr();
        }   
        
        /**
         * @return const T& 
         */
        ALWAYS_INLINE operator const T&() const {
            return *as_nonnull_ptr();
        }

        operator bool() const = delete;
        bool operator!() const = delete;

        /**
         * @param other 
         */
        void swap(NonnullRefPtr& other) {
            if (this == &other)
                return;

            T* other_ptr = other.exchange(nullptr);
            T* ptr = exchange(other_ptr);
            other.exchange(ptr);
        }

        /**
         * @tparam U 
         * @param other 
         */
        template<typename U>
        void swap(NonnullRefPtr<U>& other) {
            U* other_ptr = other.exchange(nullptr);
            T* ptr = exchange(other_ptr);
            other.exchange(ptr);
        }

    private:
        /**
         * @brief Construct a new Nonnull Ref Ptr object
         * 
         */
        NonnullRefPtr() = delete;

        /**
         * @return ALWAYS_INLINE* 
         */
        ALWAYS_INLINE T* as_ptr() const {
            return (T*)(m_bits.load(Mods::MemoryOrder::memory_order_relaxed) & ~(FlatPtr)1);
        }

        /**
         * @return ALWAYS_INLINE* 
         */
        ALWAYS_INLINE T* as_nonnull_ptr() const {
            T* ptr = (T*)(m_bits.load(Mods::MemoryOrder::memory_order_relaxed) & ~(FlatPtr)1);
            ASSERT(ptr);
            return ptr;
        }

        /**
         * @tparam F 
         * @param f 
         */
        template<typename F>
        void do_while_locked(F f) const {
    #ifdef KERNEL
            Kernel::ScopedCritical critical;
    #endif
            FlatPtr bits;
            for (;;) {
                bits = m_bits.fetch_or(1, Mods::MemoryOrder::memory_order_acq_rel);
                if (!(bits & 1))
                    break;
    #ifdef KERNEL
                Kernel::Processor::wait_check();
    #endif
            }
            ASSERT(!(bits & 1));
            f((T*)bits);
            m_bits.store(bits, Mods::MemoryOrder::memory_order_release);
        }   

        /**
         * @param new_ptr 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void assign(T* new_ptr) {
            T* prev_ptr = exchange(new_ptr);
            unref_if_not_null(prev_ptr);
        }

        /**
         * @param new_ptr 
         * @return ALWAYS_INLINE* 
         */
        ALWAYS_INLINE T* exchange(T* new_ptr) {
            ASSERT(!((FlatPtr)new_ptr & 1));
    #ifdef KERNEL
            Kernel::ScopedCritical critical;
    #endif
            FlatPtr expected = m_bits.load(Mods::MemoryOrder::memory_order_relaxed);
            for (;;) {
                expected &= ~(FlatPtr)1; 
                if (m_bits.compare_exchange_strong(expected, (FlatPtr)new_ptr, Mods::MemoryOrder::memory_order_acq_rel))
                    break;
    #ifdef KERNEL
                Kernel::Processor::wait_check();
    #endif
            }
            ASSERT(!(expected & 1));
            return (T*)expected;
        }

        /**
         * @return T* 
         */
        T* add_ref() const {
    #ifdef KERNEL
            Kernel::ScopedCritical critical;
    #endif
            FlatPtr expected = m_bits.load(Mods::MemoryOrder::memory_order_relaxed);
            for (;;) {
                expected &= ~(FlatPtr)1; 
                if (m_bits.compare_exchange_strong(expected, expected | 1, Mods::MemoryOrder::memory_order_acq_rel))
                    break;
    #ifdef KERNEL
                Kernel::Processor::wait_check();
    #endif
            }

            ref_if_not_null((T*)expected);

            m_bits.store(expected, Mods::MemoryOrder::memory_order_release);
            return (T*)expected;
        }

        mutable Atomic<FlatPtr> m_bits { 0 };
    };

    /**
     * @tparam T 
     * @param object 
     * @return NonnullRefPtr<T> 
     */
    template<typename T>
    inline NonnullRefPtr<T> adopt(T& object) {
        return NonnullRefPtr<T>(NonnullRefPtr<T>::Adopt, object);
    }

    /**
     * @tparam T 
     * @param stream 
     * @param value 
     * @return const LogStream& 
     */
    template<typename T>
    inline const LogStream& operator<<(const LogStream& stream, const NonnullRefPtr<T>& value) {
        return stream << value.ptr();
    }

    /**
     * @tparam T 
     */
    template<typename T>
    struct Formatter<NonnullRefPtr<T>> : Formatter<const T*> {
        void format(TypeErasedFormatParams& params, FormatBuilder& builder, const NonnullRefPtr<T>& value)
        {
            Formatter<const T*>::format(params, builder, value.ptr());
        }
    };

    /**
     * @tparam T 
     * @tparam U 
     * @param a 
     * @param b 
     */
    template<typename T, typename U>
    inline void swap(NonnullRefPtr<T>& a, NonnullRefPtr<U>& b) {
        a.swap(b);
    }

}

using Mods::adopt;
using Mods::NonnullRefPtr;
