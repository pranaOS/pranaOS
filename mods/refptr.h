/**
 * @file refptr.h
 * @author Krisna Pranav
 * @brief RefPtr
 * @version 6.0
 * @date 2023-07-02
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "atomic.h"
#include "logstream.h"
#include "nonnullrefptr.h"
#include "stdlibextra.h"
#include "traits.h"
#include "types.h"


#ifdef KERNEL
#    include <kernel/arch/i386/cpu.h>
#endif

namespace Mods {

    template<typename T>
    class OwnPtr;

    template<typename T>
    struct RefPtrTraits {

        /**
         * @param bits 
         * @return ALWAYS_INLINE* 
         */
        ALWAYS_INLINE static T* as_ptr(FlatPtr bits) {
            return (T*)(bits & ~(FlatPtr)1);
        }

        /**
         * @param ptr 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE static FlatPtr as_bits(T* ptr) {
            ASSERT(!((FlatPtr)ptr & 1));
            return (FlatPtr)ptr;
        }

        /**
         * @tparam U 
         * @tparam PtrTraits 
         * @param bits 
         * @return ALWAYS_INLINE 
         */
        template<typename U, typename PtrTraits>
        ALWAYS_INLINE static FlatPtr convert_from(FlatPtr bits) {
            if (PtrTraits::is_null(bits))
                return default_null_value;
            return as_bits(PtrTraits::as_ptr(bits));
        }

        /** 
         * @param bits 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE static bool is_null(FlatPtr bits) {
            return !(bits & ~(FlatPtr)1);
        }

        /**
         * @param atomic_var 
         * @param new_value 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE static FlatPtr exchange(Atomic<FlatPtr>& atomic_var, FlatPtr new_value) {
            ASSERT(!(new_value & 1));
            FlatPtr expected = atomic_var.load(Mods::MemoryOrder::memory_order_relaxed);
            for (;;) {
                expected &= ~(FlatPtr)1; 
                if (atomic_var.compare_exchange_strong(expected, new_value, Mods::MemoryOrder::memory_order_acq_rel))
                    break;
    #ifdef KERNEL
                Kernel::Processor::wait_check();
    #endif
            }
            return expected;
        }

        /**
         * @param atomic_var 
         * @param new_value 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE static bool exchange_if_null(Atomic<FlatPtr>& atomic_var, FlatPtr new_value) {
            ASSERT(!(new_value & 1));
            for (;;) {
                FlatPtr expected = default_null_value;
                if (atomic_var.compare_exchange_strong(expected, new_value, Mods::MemoryOrder::memory_order_acq_rel))
                    break;
                if (!is_null(expected))
                    return false;
    #ifdef KERNEL
                Kernel::Processor::wait_check();
    #endif
            }
            return true;
        }

        /**
         * @param atomic_var 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE static FlatPtr lock(Atomic<FlatPtr>& atomic_var) {
            FlatPtr bits;
            for (;;) {
                bits = atomic_var.fetch_or(1, Mods::MemoryOrder::memory_order_acq_rel);
                if (!(bits & 1))
                    break;
    #ifdef KERNEL
                Kernel::Processor::wait_check();
    #endif
            }
            ASSERT(!(bits & 1));
            return bits;
        }

        /**
         * @param atomic_var 
         * @param new_value 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE static void unlock(Atomic<FlatPtr>& atomic_var, FlatPtr new_value) {
            ASSERT(!(new_value & 1));
            atomic_var.store(new_value, Mods::MemoryOrder::memory_order_release);
        }

        static constexpr FlatPtr default_null_value = 0;

        using NullType = std::nullptr_t;
    };

    template<typename T, typename PtrTraits>
    class RefPtr {

        template<typename U, typename P>
        friend class RefPtr;

        template<typename U>
        friend class WeakPtr;

    public:
        enum AdoptTag {
            Adopt
        };

        RefPtr() {}

        /**
         * @param ptr 
         */
        RefPtr(const T* ptr) : m_bits(PtrTraits::as_bits(const_cast<T*>(ptr)))
        {
            ref_if_not_null(const_cast<T*>(ptr));
        }

        /**
         * @param object 
         */
        RefPtr(const T& object) : m_bits(PtrTraits::as_bits(const_cast<T*>(&object)))
        {
            T* ptr = const_cast<T*>(&object);
            ASSERT(ptr);
            ASSERT(!is_null());
            ptr->ref();
        }

        /**
         * @param object 
         */
        RefPtr(AdoptTag, T& object) : m_bits(PtrTraits::as_bits(&object))
        {
            ASSERT(!is_null());
        }

        /**
         * @param other 
         */
        RefPtr(RefPtr&& other) : m_bits(other.leak_ref_raw())
        {
        }

        /**
         * @param other 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE RefPtr(const NonnullRefPtr<T>& other)
            : m_bits(PtrTraits::as_bits(const_cast<T*>(other.add_ref())))
        {
        }

        /**
         * @tparam U 
         * @param other 
         * @return ALWAYS_INLINE 
         */
        template<typename U>
        ALWAYS_INLINE RefPtr(const NonnullRefPtr<U>& other)
            : m_bits(PtrTraits::as_bits(const_cast<U*>(other.add_ref())))
        {
        }

        /**
         * @tparam U 
         * @param other 
         * @return ALWAYS_INLINE 
         */
        template<typename U>
        ALWAYS_INLINE RefPtr(NonnullRefPtr<U>&& other)
            : m_bits(PtrTraits::as_bits(&other.leak_ref()))
        {
            ASSERT(!is_null());
        }

        /**
         * @tparam U 
         * @tparam P 
         * @param other 
         */
        template<typename U, typename P = RefPtrTraits<U>>
        RefPtr(RefPtr<U, P>&& other)
            : m_bits(PtrTraits::template convert_from<U, P>(other.leak_ref_raw()))
        {
        }

        /**
         * @param other 
         */
        RefPtr(const RefPtr& other)
            : m_bits(other.add_ref_raw())
        {
        }


        /**
         * @tparam U 
         * @tparam P 
         * @param other 
         */
        template<typename U, typename P = RefPtrTraits<U>>
        RefPtr(const RefPtr<U, P>& other)
            : m_bits(other.add_ref_raw())
        {
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE ~RefPtr() {
            clear();
    #ifdef SANITIZE_PTRS
            if constexpr (sizeof(T*) == 8)
                m_bits.store(0xe0e0e0e0e0e0e0e0, Mods::MemoryOrder::memory_order_relaxed);
            else
                m_bits.store(0xe0e0e0e0, Mods::MemoryOrder::memory_order_relaxed);
    #endif
        }
        RefPtr(std::nullptr_t) { }

        template<typename U>
        RefPtr(const OwnPtr<U>&) = delete;
        template<typename U>
        RefPtr& operator=(const OwnPtr<U>&) = delete;

        /**
         * @param other 
         */
        void swap(RefPtr& other) {
            if (this == &other)
                return;

            FlatPtr other_bits = PtrTraits::exchange(other.m_bits, PtrTraits::default_null_value);
            FlatPtr bits = PtrTraits::exchange(m_bits, other_bits);
            PtrTraits::exchange(other.m_bits, bits);
        }

        /**
         * @tparam U 
         * @tparam P 
         * @param other 
         */
        template<typename U, typename P = RefPtrTraits<U>>
        void swap(RefPtr<U, P>& other) {
            FlatPtr other_bits = P::exchange(other.m_bits, P::default_null_value);
            FlatPtr bits = PtrTraits::exchange(m_bits, PtrTraits::template convert_from<U, P>(other_bits));
            P::exchange(other.m_bits, P::template convert_from<U, P>(bits));
        }

        /**
         * @param other 
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE RefPtr& operator=(RefPtr&& other) {
            if (this != &other)
                assign_raw(other.leak_ref_raw());
            return *this;
        }

        /**
         * @tparam U 
         * @tparam P 
         * @param other 
         * @return ALWAYS_INLINE& 
         */
        template<typename U, typename P = RefPtrTraits<U>>
        ALWAYS_INLINE RefPtr& operator=(RefPtr<U, P>&& other) {
            assign_raw(PtrTraits::template convert_from<U, P>(other.leak_ref_raw()));
            return *this;
        }

        /**
         * @tparam U 
         * @param other 
         * @return ALWAYS_INLINE& 
         */
        template<typename U>
        ALWAYS_INLINE RefPtr& operator=(NonnullRefPtr<U>&& other) {
            assign_raw(PtrTraits::as_bits(&other.leak_ref()));
            return *this;
        }

        /**
         * @param other 
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE RefPtr& operator=(const NonnullRefPtr<T>& other) {
            assign_raw(PtrTraits::as_bits(other.add_ref()));
            return *this;
        }

        /**
         * @tparam U 
         * @param other 
         * @return ALWAYS_INLINE& 
         */
        template<typename U>
        ALWAYS_INLINE RefPtr& operator=(const NonnullRefPtr<U>& other) {
            assign_raw(PtrTraits::as_bits(other.add_ref()));
            return *this;
        }

        /**
         * @param other 
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE RefPtr& operator=(const RefPtr& other) {
            if (this != &other)
                assign_raw(other.add_ref_raw());
            return *this;
        }

        /**
         * @tparam U 
         * @param other 
         * @return ALWAYS_INLINE& 
         */
        template<typename U>
        ALWAYS_INLINE RefPtr& operator=(const RefPtr<U>& other) {
            assign_raw(other.add_ref_raw());
            return *this;
        }

        /**
         * @param ptr 
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE RefPtr& operator=(const T* ptr) {
            ref_if_not_null(const_cast<T*>(ptr));
            assign_raw(PtrTraits::as_bits(const_cast<T*>(ptr)));
            return *this;
        }

        /**
         * @param object 
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE RefPtr& operator=(const T& object) {
            const_cast<T&>(object).ref();
            assign_raw(PtrTraits::as_bits(const_cast<T*>(&object)));
            return *this;
        }

        /**
         * @return RefPtr& 
         */
        RefPtr& operator=(std::nullptr_t) {
            clear();
            return *this;
        }

        /**
         * @param other 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE bool assign_if_null(RefPtr&& other) {
            if (this == &other)
                return is_null();
            return PtrTraits::exchange_if_null(m_bits, other.leak_ref_raw());
        }

        /**
         * @tparam U 
         * @tparam P 
         * @param other 
         * @return ALWAYS_INLINE 
         */
        template<typename U, typename P = RefPtrTraits<U>>
        ALWAYS_INLINE bool assign_if_null(RefPtr<U, P>&& other) {
            if (this == &other)
                return is_null();
            return PtrTraits::exchange_if_null(m_bits, PtrTraits::template convert_from<U, P>(other.leak_ref_raw()));
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void clear() {
            assign_raw(PtrTraits::default_null_value);
        }

        /**
         * @return true 
         * @return false 
         */
        bool operator!() const { 
            return PtrTraits::is_null(m_bits.load(Mods::MemoryOrder::memory_order_relaxed)); 
        }

        /**
         * @return T* 
         */
        [[nodiscard]] T* leak_ref() {
            FlatPtr bits = PtrTraits::exchange(m_bits, PtrTraits::default_null_value);
            return PtrTraits::as_ptr(bits);
        }

        /**
         * @return NonnullRefPtr<T> 
         */
        NonnullRefPtr<T> release_nonnull() {
            FlatPtr bits = PtrTraits::exchange(m_bits, PtrTraits::default_null_value);
            ASSERT(!PtrTraits::is_null(bits));
            return NonnullRefPtr<T>(NonnullRefPtr<T>::Adopt, *PtrTraits::as_ptr(bits));
        }

        /**
         * @return ALWAYS_INLINE* 
         */
        ALWAYS_INLINE T* ptr() { 
            return as_ptr();   
        }

        /**
         * @return ALWAYS_INLINE const* 
         */
        ALWAYS_INLINE const T* ptr() const { 
            return as_ptr(); 
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
         * @return const T* 
         */
        ALWAYS_INLINE operator const T*() const { 
            return as_ptr(); 
        }

        /** 
         * @return T* 
         */
        ALWAYS_INLINE operator T*() { 
            return as_ptr(); 
        }

        /**
         * @return true 
         * @return false 
         */
        ALWAYS_INLINE operator bool() { 
            return !is_null(); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool operator==(std::nullptr_t) const { 
            return is_null(); 
        }

        /** 
         * @return true 
         * @return false 
         */
        bool operator!=(std::nullptr_t) const { 
            return !is_null(); 
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(const RefPtr& other) const { 
            return as_ptr() == other.as_ptr();
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(const RefPtr& other) const { 
            return as_ptr() != other.as_ptr(); 
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(RefPtr& other) { 
            return as_ptr() == other.as_ptr(); 
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(RefPtr& other) { return as_ptr() != other.as_ptr(); }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(const T* other) const { 
            return as_ptr() == other; 
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(const T* other) const { 
            return as_ptr() != other; 
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(T* other) { 
            return as_ptr() == other; 
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(T* other) { 
            return as_ptr() != other; 
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE bool is_null() const { 
            return PtrTraits::is_null(m_bits.load(Mods::MemoryOrder::memory_order_relaxed)); 
        }

        /**
         * @tparam U 
         * @tparam EnableIf<IsSame<U, T>::value && !IsNullPointer<typename PtrTraits::NullType>::value>::Type 
         * @return PtrTraits::NullType 
         */
        template<typename U = T, typename EnableIf<IsSame<U, T>::value && !IsNullPointer<typename PtrTraits::NullType>::value>::Type* = nullptr>
        typename PtrTraits::NullType null_value() const {
            FlatPtr bits = m_bits.load(Mods::MemoryOrder::memory_order_relaxed);
            ASSERT(PtrTraits::is_null(bits));
            return PtrTraits::to_null_value(bits);
        }

        /**
         * @tparam U 
         * @tparam EnableIf<IsSame<U, T>::value && !IsNullPointer<typename PtrTraits::NullType>::value>::Type 
         * @param value 
         */
        template<typename U = T, typename EnableIf<IsSame<U, T>::value && !IsNullPointer<typename PtrTraits::NullType>::value>::Type* = nullptr>
        void set_null_value(typename PtrTraits::NullType value) {
            FlatPtr bits = PtrTraits::from_null_value(value);
            ASSERT(PtrTraits::is_null(bits));
            assign_raw(bits);
        }

    private:
        /**
         * @tparam F 
         * @param f 
         */
        template<typename F>
        void do_while_locked(F f) const {
    #ifdef KERNEL
            Kernel::ScopedCritical critical;
    #endif
            FlatPtr bits = PtrTraits::lock(m_bits);
            T* ptr = PtrTraits::as_ptr(bits);
            f(ptr);
            PtrTraits::unlock(m_bits, bits);
        }

        /**
         * @return ALWAYS_INLINE 
         */
        [[nodiscard]] ALWAYS_INLINE FlatPtr leak_ref_raw() {
            return PtrTraits::exchange(m_bits, PtrTraits::default_null_value);
        }

        /**
         * @brief add_ref_raw
         * 
         * @return ALWAYS_INLINE 
         */
        [[nodiscard]] ALWAYS_INLINE FlatPtr add_ref_raw() const {
    #ifdef KERNEL
            Kernel::ScopedCritical critical;
    #endif
            FlatPtr bits = PtrTraits::lock(m_bits);
            if (T* ptr = PtrTraits::as_ptr(bits))
                ptr->ref();
            PtrTraits::unlock(m_bits, bits);
            return bits;
        }

        /**
         * @brief assign_raw
         * 
         * @param bits 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void assign_raw(FlatPtr bits) {
            FlatPtr prev_bits = PtrTraits::exchange(m_bits, bits);
            unref_if_not_null(PtrTraits::as_ptr(prev_bits));
        }
        
        /**
         * @brief as_ptr
         * 
         * @return ALWAYS_INLINE* 
         */
        ALWAYS_INLINE T* as_ptr() const {
            return PtrTraits::as_ptr(m_bits.load(Mods::MemoryOrder::memory_order_relaxed));
        }

        /**
         * @brief as_nonnull_ptr
         * 
         * @return ALWAYS_INLINE* 
         */
        ALWAYS_INLINE T* as_nonnull_ptr() const {
            return as_nonnull_ptr(m_bits.load(Mods::MemoryOrder::memory_order_relaxed));
        }

        /**
         * @param bits 
         * @return ALWAYS_INLINE* 
         */
        ALWAYS_INLINE T* as_nonnull_ptr(FlatPtr bits) const {
            ASSERT(!PtrTraits::is_null(bits));
            return PtrTraits::as_ptr(bits);
        }

        mutable Atomic<FlatPtr> m_bits { PtrTraits::default_null_value };
    };

    /** 
     * @tparam T 
     * @tparam PtrTraits 
     * @param stream 
     * @param value 
     * @return const LogStream& 
     */
    template<typename T, typename PtrTraits = RefPtrTraits<T>>
    inline const LogStream& operator<<(const LogStream& stream, const RefPtr<T, PtrTraits>& value) {
        return stream << value.ptr();
    }

    /**
     * @tparam T 
     */
    template<typename T>
    struct Traits<RefPtr<T>> : public GenericTraits<RefPtr<T>> {
        using PeekType = const T*;
        static unsigned hash(const RefPtr<T>& p) { return ptr_hash(p.ptr()); }
        static bool equals(const RefPtr<T>& a, const RefPtr<T>& b) { return a.ptr() == b.ptr(); }
    };

    /**
     * @tparam T 
     * @tparam U 
     * @param ptr 
     * @return NonnullRefPtr<T> 
     */
    template<typename T, typename U>
    inline NonnullRefPtr<T> static_ptr_cast(const NonnullRefPtr<U>& ptr) {
        return NonnullRefPtr<T>(static_cast<const T&>(*ptr));
    }

    /**
     * @tparam T 
     * @tparam U 
     * @tparam PtrTraits 
     * @param ptr 
     * @return RefPtr<T> 
     */
    template<typename T, typename U, typename PtrTraits = RefPtrTraits<T>>
    inline RefPtr<T> static_ptr_cast(const RefPtr<U>& ptr) {
        return RefPtr<T, PtrTraits>(static_cast<const T*>(ptr.ptr()));
    }
    
    /**
     * @tparam T 
     * @tparam PtrTraitsT 
     * @tparam U 
     * @tparam PtrTraitsU 
     * @param a 
     * @param b 
     */
    template<typename T, typename PtrTraitsT, typename U, typename PtrTraitsU>
    inline void swap(RefPtr<T, PtrTraitsT>& a, RefPtr<U, PtrTraitsU>& b) {
        a.swap(b);
    }

}

using Mods::RefPtr;
using Mods::static_ptr_cast;
