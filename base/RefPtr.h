/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Assertions.h>
#include <base/Atomic.h>
#include <base/Format.h>
#include <base/NonnullRefPtr.h>
#include <base/StdLibExtras.h>
#include <base/Traits.h>
#include <base/Types.h>
#ifdef KERNEL
#    include <kernel/Arch/x86/Processor.h>
#    include <kernel/Arch/x86/ScopedCritical.h>
#endif

namespace Base {

template<typename T>
class OwnPtr;

template<typename T>
struct RefPtrTraits {
    ALWAYS_INLINE static T* as_ptr(FlatPtr bits)
    {
        return (T*)(bits & ~(FlatPtr)1);
    }

    ALWAYS_INLINE static FlatPtr as_bits(T* ptr)
    {
        VERIFY(!((FlatPtr)ptr & 1));
        return (FlatPtr)ptr;
    }

    template<typename U, typename PtrTraits>
    ALWAYS_INLINE static FlatPtr convert_from(FlatPtr bits)
    {
        if (PtrTraits::is_null(bits))
            return default_null_value;
        return as_bits(PtrTraits::as_ptr(bits));
    }

    ALWAYS_INLINE static bool is_null(FlatPtr bits)
    {
        return !(bits & ~(FlatPtr)1);
    }

    ALWAYS_INLINE static FlatPtr exchange(Atomic<FlatPtr>& atomic_var, FlatPtr new_value)
    {
        VERIFY(!(new_value & 1));
        FlatPtr expected = atomic_var.load(Base::MemoryOrder::memory_order_relaxed);
        for (;;) {
            expected &= ~(FlatPtr)1; 
            if (atomic_var.compare_exchange_strong(expected, new_value, Base::MemoryOrder::memory_order_acq_rel))
                break;
#ifdef KERNEL
            Kernel::Processor::wait_check();
#endif
        }
        return expected;
    }

    ALWAYS_INLINE static bool exchange_if_null(Atomic<FlatPtr>& atomic_var, FlatPtr new_value)
    {
        VERIFY(!(new_value & 1));
        for (;;) {
            FlatPtr expected = default_null_value; 
            if (atomic_var.compare_exchange_strong(expected, new_value, Base::MemoryOrder::memory_order_acq_rel))
                break;
            if (!is_null(expected))
                return false;
#ifdef KERNEL
            Kernel::Processor::wait_check();
#endif
        }
        return true;
    }

    ALWAYS_INLINE static FlatPtr lock(Atomic<FlatPtr>& atomic_var)
    {

        FlatPtr bits;
        for (;;) {
            bits = atomic_var.fetch_or(1, Base::MemoryOrder::memory_order_acq_rel);
            if (!(bits & 1))
                break;
#ifdef KERNEL
            Kernel::Processor::wait_check();
#endif
        }
        VERIFY(!(bits & 1));
        return bits;
    }

    ALWAYS_INLINE static void unlock(Atomic<FlatPtr>& atomic_var, FlatPtr new_value)
    {
        VERIFY(!(new_value & 1));
        atomic_var.store(new_value, Base::MemoryOrder::memory_order_release);
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

    RefPtr() = default;
    RefPtr(const T* ptr)
        : m_bits(PtrTraits::as_bits(const_cast<T*>(ptr)))
    {
        ref_if_not_null(const_cast<T*>(ptr));
    }
    RefPtr(const T& object)
        : m_bits(PtrTraits::as_bits(const_cast<T*>(&object)))
    {
        T* ptr = const_cast<T*>(&object);
        VERIFY(ptr);
        VERIFY(!is_null());
        ptr->ref();
    }
    RefPtr(AdoptTag, T& object)
        : m_bits(PtrTraits::as_bits(&object))
    {
        VERIFY(!is_null());
    }
    RefPtr(RefPtr&& other)
        : m_bits(other.leak_ref_raw())
    {
    }
    ALWAYS_INLINE RefPtr(const NonnullRefPtr<T>& other)
        : m_bits(PtrTraits::as_bits(const_cast<T*>(other.add_ref())))
    {
    }
    template<typename U>
    ALWAYS_INLINE RefPtr(const NonnullRefPtr<U>& other)
        : m_bits(PtrTraits::as_bits(const_cast<U*>(other.add_ref())))
    {
    }
    template<typename U>
    ALWAYS_INLINE RefPtr(NonnullRefPtr<U>&& other)
        : m_bits(PtrTraits::as_bits(&other.leak_ref()))
    {
        VERIFY(!is_null());
    }
    template<typename U, typename P = RefPtrTraits<U>>
    RefPtr(RefPtr<U, P>&& other)
        : m_bits(PtrTraits::template convert_from<U, P>(other.leak_ref_raw()))
    {
    }
    RefPtr(const RefPtr& other)
        : m_bits(other.add_ref_raw())
    {
    }
    template<typename U, typename P = RefPtrTraits<U>>
    RefPtr(const RefPtr<U, P>& other)
        : m_bits(other.add_ref_raw())
    {
    }
    ALWAYS_INLINE ~RefPtr()
    {
        clear();
#ifdef SANITIZE_PTRS
        if constexpr (sizeof(T*) == 8)
            m_bits.store(0xe0e0e0e0e0e0e0e0, Base::MemoryOrder::memory_order_relaxed);
        else
            m_bits.store(0xe0e0e0e0, Base::MemoryOrder::memory_order_relaxed);
#endif
    }

    template<typename U>
    RefPtr(const OwnPtr<U>&) = delete;
    template<typename U>
    RefPtr& operator=(const OwnPtr<U>&) = delete;

    void swap(RefPtr& other)
    {
        if (this == &other)
            return;

        FlatPtr other_bits = PtrTraits::exchange(other.m_bits, PtrTraits::default_null_value);
        FlatPtr bits = PtrTraits::exchange(m_bits, other_bits);
        PtrTraits::exchange(other.m_bits, bits);
    }

    template<typename U, typename P = RefPtrTraits<U>>
    void swap(RefPtr<U, P>& other)
    {
        FlatPtr other_bits = P::exchange(other.m_bits, P::default_null_value);
        FlatPtr bits = PtrTraits::exchange(m_bits, PtrTraits::template convert_from<U, P>(other_bits));
        P::exchange(other.m_bits, P::template convert_from<U, P>(bits));
    }

    ALWAYS_INLINE RefPtr& operator=(RefPtr&& other)
    {
        if (this != &other)
            assign_raw(other.leak_ref_raw());
        return *this;
    }

    template<typename U, typename P = RefPtrTraits<U>>
    ALWAYS_INLINE RefPtr& operator=(RefPtr<U, P>&& other)
    {
        assign_raw(PtrTraits::template convert_from<U, P>(other.leak_ref_raw()));
        return *this;
    }

    template<typename U>
    ALWAYS_INLINE RefPtr& operator=(NonnullRefPtr<U>&& other)
    {
        assign_raw(PtrTraits::as_bits(&other.leak_ref()));
        return *this;
    }

    ALWAYS_INLINE RefPtr& operator=(const NonnullRefPtr<T>& other)
    {
        assign_raw(PtrTraits::as_bits(other.add_ref()));
        return *this;
    }

    template<typename U>
    ALWAYS_INLINE RefPtr& operator=(const NonnullRefPtr<U>& other)
    {
        assign_raw(PtrTraits::as_bits(other.add_ref()));
        return *this;
    }

    ALWAYS_INLINE RefPtr& operator=(const RefPtr& other)
    {
        if (this != &other)
            assign_raw(other.add_ref_raw());
        return *this;
    }

    template<typename U>
    ALWAYS_INLINE RefPtr& operator=(const RefPtr<U>& other)
    {
        assign_raw(other.add_ref_raw());
        return *this;
    }

    ALWAYS_INLINE RefPtr& operator=(const T* ptr)
    {
        ref_if_not_null(const_cast<T*>(ptr));
        assign_raw(PtrTraits::as_bits(const_cast<T*>(ptr)));
        return *this;
    }

    ALWAYS_INLINE RefPtr& operator=(const T& object)
    {
        const_cast<T&>(object).ref();
        assign_raw(PtrTraits::as_bits(const_cast<T*>(&object)));
        return *this;
    }

    RefPtr& operator=(std::nullptr_t)
    {
        clear();
        return *this;
    }

    ALWAYS_INLINE bool assign_if_null(RefPtr&& other)
    {
        if (this == &other)
            return is_null();
        return PtrTraits::exchange_if_null(m_bits, other.leak_ref_raw());
    }

    template<typename U, typename P = RefPtrTraits<U>>
    ALWAYS_INLINE bool assign_if_null(RefPtr<U, P>&& other)
    {
        if (this == &other)
            return is_null();
        return PtrTraits::exchange_if_null(m_bits, PtrTraits::template convert_from<U, P>(other.leak_ref_raw()));
    }

    ALWAYS_INLINE void clear()
    {
        assign_raw(PtrTraits::default_null_value);
    }

    bool operator!() const { return PtrTraits::is_null(m_bits.load(Base::MemoryOrder::memory_order_relaxed)); }

    [[nodiscard]] T* leak_ref()
    {
        FlatPtr bits = PtrTraits::exchange(m_bits, PtrTraits::default_null_value);
        return PtrTraits::as_ptr(bits);
    }

    NonnullRefPtr<T> release_nonnull()
    {
        FlatPtr bits = PtrTraits::exchange(m_bits, PtrTraits::default_null_value);
        VERIFY(!PtrTraits::is_null(bits));
        return NonnullRefPtr<T>(NonnullRefPtr<T>::Adopt, *PtrTraits::as_ptr(bits));
    }

    ALWAYS_INLINE T* ptr() { return as_ptr(); }
    ALWAYS_INLINE const T* ptr() const { return as_ptr(); }

    ALWAYS_INLINE T* operator->()
    {
        return as_nonnull_ptr();
    }

    ALWAYS_INLINE const T* operator->() const
    {
        return as_nonnull_ptr();
    }

    ALWAYS_INLINE T& operator*()
    {
        return *as_nonnull_ptr();
    }

    ALWAYS_INLINE const T& operator*() const
    {
        return *as_nonnull_ptr();
    }

    ALWAYS_INLINE operator const T*() const { return as_ptr(); }
    ALWAYS_INLINE operator T*() { return as_ptr(); }

    ALWAYS_INLINE operator bool() { return !is_null(); }

    bool operator==(std::nullptr_t) const { return is_null(); }
    bool operator!=(std::nullptr_t) const { return !is_null(); }

    bool operator==(const RefPtr& other) const { return as_ptr() == other.as_ptr(); }
    bool operator!=(const RefPtr& other) const { return as_ptr() != other.as_ptr(); }

    bool operator==(RefPtr& other) { return as_ptr() == other.as_ptr(); }
    bool operator!=(RefPtr& other) { return as_ptr() != other.as_ptr(); }

    bool operator==(const T* other) const { return as_ptr() == other; }
    bool operator!=(const T* other) const { return as_ptr() != other; }

    bool operator==(T* other) { return as_ptr() == other; }
    bool operator!=(T* other) { return as_ptr() != other; }

    ALWAYS_INLINE bool is_null() const { return PtrTraits::is_null(m_bits.load(Base::MemoryOrder::memory_order_relaxed)); }

    template<typename U = T, typename EnableIf<IsSame<U, T> && !IsNullPointer<typename PtrTraits::NullType>>::Type* = nullptr>
    typename PtrTraits::NullType null_value() const
    {
        FlatPtr bits = m_bits.load(Base::MemoryOrder::memory_order_relaxed);
        VERIFY(PtrTraits::is_null(bits));
        return PtrTraits::to_null_value(bits);
    }
    template<typename U = T, typename EnableIf<IsSame<U, T> && !IsNullPointer<typename PtrTraits::NullType>>::Type* = nullptr>
    void set_null_value(typename PtrTraits::NullType value)
    {
        FlatPtr bits = PtrTraits::from_null_value(value);
        VERIFY(PtrTraits::is_null(bits));
        assign_raw(bits);
    }

private:
    template<typename F>
    void do_while_locked(F f) const
    {
#ifdef KERNEL
        Kernel::ScopedCritical critical;
#endif
        FlatPtr bits = PtrTraits::lock(m_bits);
        T* ptr = PtrTraits::as_ptr(bits);
        f(ptr);
        PtrTraits::unlock(m_bits, bits);
    }

    [[nodiscard]] ALWAYS_INLINE FlatPtr leak_ref_raw()
    {
        return PtrTraits::exchange(m_bits, PtrTraits::default_null_value);
    }

    [[nodiscard]] ALWAYS_INLINE FlatPtr add_ref_raw() const
    {
#ifdef KERNEL
        Kernel::ScopedCritical critical;
#endif

        FlatPtr bits = PtrTraits::lock(m_bits);
        if (T* ptr = PtrTraits::as_ptr(bits))
            ptr->ref();
        PtrTraits::unlock(m_bits, bits);
        return bits;
    }

    ALWAYS_INLINE void assign_raw(FlatPtr bits)
    {
        FlatPtr prev_bits = PtrTraits::exchange(m_bits, bits);
        unref_if_not_null(PtrTraits::as_ptr(prev_bits));
    }

    ALWAYS_INLINE T* as_ptr() const
    {
        return PtrTraits::as_ptr(m_bits.load(Base::MemoryOrder::memory_order_relaxed));
    }

    ALWAYS_INLINE T* as_nonnull_ptr() const
    {
        return as_nonnull_ptr(m_bits.load(Base::MemoryOrder::memory_order_relaxed));
    }

    ALWAYS_INLINE T* as_nonnull_ptr(FlatPtr bits) const
    {
        VERIFY(!PtrTraits::is_null(bits));
        return PtrTraits::as_ptr(bits);
    }

    mutable Atomic<FlatPtr> m_bits { PtrTraits::default_null_value };
};

template<typename T>
struct Formatter<RefPtr<T>> : Formatter<const T*> {
    void format(FormatBuilder& builder, const RefPtr<T>& value)
    {
        Formatter<const T*>::format(builder, value.ptr());
    }
};

template<typename T>
struct Traits<RefPtr<T>> : public GenericTraits<RefPtr<T>> {
    using PeekType = T*;
    using ConstPeekType = const T*;
    static unsigned hash(const RefPtr<T>& p) { return ptr_hash(p.ptr()); }
    static bool equals(const RefPtr<T>& a, const RefPtr<T>& b) { return a.ptr() == b.ptr(); }
};

template<typename T, typename U>
inline NonnullRefPtr<T> static_ptr_cast(const NonnullRefPtr<U>& ptr)
{
    return NonnullRefPtr<T>(static_cast<const T&>(*ptr));
}

template<typename T, typename U, typename PtrTraits = RefPtrTraits<T>>
inline RefPtr<T> static_ptr_cast(const RefPtr<U>& ptr)
{
    return RefPtr<T, PtrTraits>(static_cast<const T*>(ptr.ptr()));
}

template<typename T, typename PtrTraitsT, typename U, typename PtrTraitsU>
inline void swap(RefPtr<T, PtrTraitsT>& a, RefPtr<U, PtrTraitsU>& b)
{
    a.swap(b);
}

template<typename T>
inline RefPtr<T> adopt_ref_if_nonnull(T* object)
{
    if (object)
        return RefPtr<T>(RefPtr<T>::Adopt, *object);
    return {};
}

template<typename T, class... Args>
requires(IsConstructible<T, Args...>) inline RefPtr<T> try_create(Args&&... args)
{
    return adopt_ref_if_nonnull(new (nothrow) T(forward<Args>(args)...));
}

template<typename T, class... Args>
inline RefPtr<T> try_create(Args&&... args)
{
    return adopt_ref_if_nonnull(new (nothrow) T { forward<Args>(args)... });
}

}

using Base::adopt_ref_if_nonnull;
using Base::RefPtr;
using Base::static_ptr_cast;
using Base::try_create;