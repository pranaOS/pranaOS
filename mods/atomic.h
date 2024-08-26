/**
 * @file atomic.h
 * @author Krisna Pranav
 * @brief Atomic
 * @version 6.0
 * @date 2023-06-29
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "concept.h"
#include "platform.h"
#include "types.h"

namespace Mods
{
    /**
     * @param order 
     */
    static inline void atomic_signal_fence(MemoryOrder order) noexcept
    {
        return __atomic_signal_fence(order);
    }

    /**
     * @param order 
     */
    static inline void atomic_thread_fence(MemoryOrder order) noexcept
    {
        return __atomic_thread_fence(order);
    }

    static inline void full_memory_barrier() noexcept
    {
        atomic_signal_fence(Mods::MemoryOrder::memory_order_acq_rel);
        atomic_thread_fence(Mods::MemoryOrder::memory_order_acq_rel);
    }

    /**
     * @tparam T 
     * @param var 
     * @param desired 
     * @param order 
     * @return T 
     */
    template<typename T>
    static inline T atomic_exchange(volatile T* var, T desired, MemoryOrder order = memory_order_seq_cst) noexcept
    {
        return __atomic_exchange_n(var, desired, order);
    }

    /**
     * @tparam T 
     * @tparam V 
     * @param var 
     * @param desired 
     * @param order 
     * @return V* 
     */
    template<typename T, typename V = RemoveVolatile<T>>
    static inline V* atomic_exchange(volatile T** var, V* desired, MemoryOrder order = memory_order_seq_cst) noexcept
    {
        return __atomic_exchange_n(var, desired, order);
    }

    /**
     * @tparam T 
     * @tparam V 
     * @param var 
     * @param order 
     * @return V* 
     */
    template<typename T, typename V = RemoveVolatile<T>>
    static inline V* atomic_exchange(volatile T** var, std::nullptr_t, MemoryOrder order = memory_order_seq_cst) noexcept
    {
        return __atomic_exchange_n(const_cast<V**>(var), nullptr, order);
    }

    /**
     * @tparam T 
     * @param var 
     * @param expected 
     * @param desired 
     * @param order 
     * @return true 
     * @return false 
     */
    template<typename T>
    [[nodiscard]] static inline bool atomic_compare_exchange_strong(volatile T* var, T& expected, T desired, MemoryOrder order = memory_order_seq_cst) noexcept
    {
        if (order == memory_order_acq_rel || order == memory_order_release)
            return __atomic_compare_exchange_n(var, &expected, desired, false, memory_order_release, memory_order_acquire);
        return __atomic_compare_exchange_n(var, &expected, desired, false, order, order);
    }

    /**
     * @tparam T 
     * @tparam V 
     * @param var 
     * @param expected 
     * @param desired 
     * @param order 
     * @return true 
     * @return false 
     */
    template<typename T, typename V = RemoveVolatile<T>>
    [[nodiscard]] static inline bool atomic_compare_exchange_strong(volatile T** var, V*& expected, V* desired, MemoryOrder order = memory_order_seq_cst) noexcept
    {
        if (order == memory_order_acq_rel || order == memory_order_release)
            return __atomic_compare_exchange_n(var, &expected, desired, false, memory_order_release, memory_order_acquire);
        return __atomic_compare_exchange_n(var, &expected, desired, false, order, order);
    }

    /**
     * @tparam T 
     * @tparam V 
     * @param var 
     * @param expected 
     * @param order 
     * @return true 
     * @return false 
     */
    template<typename T, typename V = RemoveVolatile<T>>
    [[nodiscard]] static inline bool atomic_compare_exchange_strong(volatile T** var, V*& expected, std::nullptr_t, MemoryOrder order = memory_order_seq_cst) noexcept
    {
        if (order == memory_order_acq_rel || order == memory_order_release)
            return __atomic_compare_exchange_n(const_cast<V**>(var), &expected, nullptr, false, memory_order_release, memory_order_acquire);
        return __atomic_compare_exchange_n(const_cast<V**>(var), &expected, nullptr, false, order, order);
    }

    /**
     * @tparam T 
     * @param var 
     * @param val 
     * @param order 
     * @return T 
     */
    template<typename T>
    static inline T atomic_fetch_add(volatile T* var, T val, MemoryOrder order = memory_order_seq_cst) noexcept
    {
        return __atomic_fetch_add(var, val, order);
    }

    /**
     * @tparam T 
     * @param var 
     * @param val 
     * @param order 
     * @return T 
     */
    template<typename T>
    static inline T atomic_fetch_sub(volatile T* var, T val, MemoryOrder order = memory_order_seq_cst) noexcept
    {
        return __atomic_fetch_sub(var, val, order);
    }

    /**
     * @tparam T 
     * @param var 
     * @param val 
     * @param order 
     * @return T 
     */
    template<typename T>
    static inline T atomic_fetch_and(volatile T* var, T val, MemoryOrder order = memory_order_seq_cst) noexcept
    {
        return __atomic_fetch_and(var, val, order);
    }

    /**
     * @tparam T 
     * @param var 
     * @param val 
     * @param order 
     * @return T 
     */
    template<typename T>
    static inline T atomic_fetch_or(volatile T* var, T val, MemoryOrder order = memory_order_seq_cst) noexcept
    {
        return __atomic_fetch_or(var, val, order);
    }

    /**
     * @tparam T 
     * @param var 
     * @param val 
     * @param order 
     * @return T 
     */
    template<typename T>
    static inline T atomic_fetch_xor(volatile T* var, T val, MemoryOrder order = memory_order_seq_cst) noexcept
    {
        return __atomic_fetch_xor(var, val, order);
    }

    /**
     * @tparam T 
     * @param var 
     * @param order 
     * @return T 
     */
    template<typename T>
    static inline T atomic_load(volatile T* var, MemoryOrder order = memory_order_seq_cst) noexcept
    {
        return __atomic_load_n(var, order);
    }

    /**
     * @tparam T 
     * @tparam V 
     * @param var 
     * @param order 
     * @return V* 
     */
    template<typename T, typename V = RemoveVolatile<T>>
    static inline V* atomic_load(volatile T** var, MemoryOrder order = memory_order_seq_cst) noexcept
    {
        return __atomic_load_n(const_cast<V**>(var), order);
    }

    /**
     * @tparam T 
     * @param var 
     * @param desired 
     * @param order 
     */
    template<typename T>
    static inline void atomic_store(volatile T* var, T desired, MemoryOrder order = memory_order_seq_cst) noexcept
    {
        __atomic_store_n(var, desired, order);
    }

    /**
     * @tparam T 
     * @tparam V 
     * @param var 
     * @param desired 
     * @param order 
     */
    template<typename T, typename V = RemoveVolatile<T>>
    static inline void atomic_store(volatile T** var, V* desired, MemoryOrder order = memory_order_seq_cst) noexcept
    {
        __atomic_store_n(var, desired, order);
    }

    /**
     * @tparam T 
     * @tparam V 
     * @param var 
     * @param order 
     */
    template<typename T, typename V = RemoveVolatile<T>>
    static inline void atomic_store(volatile T** var, std::nullptr_t, MemoryOrder order = memory_order_seq_cst) noexcept
    {
        __atomic_store_n(const_cast<V**>(var), nullptr, order);
    }

    /**
     * @tparam T 
     * @param ptr 
     * @return true 
     * @return false 
     */
    template<typename T>
    static inline bool atomic_is_lock_free(volatile T* ptr = nullptr) noexcept
    {
        return __atomic_is_lock_free(sizeof(T), ptr);
    }

    /**
     * @tparam T 
     * @tparam DefaultMemoryOrder 
     */
    template<typename T, MemoryOrder DefaultMemoryOrder = Mods::MemoryOrder::memory_order_seq_cst>
    class Atomic 
    {
        T m_value { 0 };

    public:
        /**
         * @brief Construct a new Atomic object
         * 
         */
        Atomic() noexcept = default;

        /**
         * @return Atomic& 
         */
        Atomic& operator=(const Atomic&) volatile = delete;

        /**
         * @return * Atomic& 
         */
        Atomic& operator=(Atomic&&) volatile = delete;

        /**
         * @brief Construct a new Atomic object
         * 
         */
        Atomic(const Atomic&) = delete;

        /**
         * @brief Construct a new Atomic object
         * 
         */
        Atomic(Atomic&&) = delete;

        constexpr Atomic(T val) noexcept
            : m_value(val)
        { }

        /**
         * @return volatile* 
         */
        volatile T* ptr() noexcept
        {
            return &m_value;
        }

        /**
         * @param desired 
         * @param order 
         * @return T 
         */
        T exchange(T desired, MemoryOrder order = DefaultMemoryOrder) volatile noexcept
        {
            alignas(T) u8 buffer[sizeof(T)];
            T* ret = reinterpret_cast<T*>(buffer);
            __atomic_exchange(&m_value, &desired, ret, order);
            return *ret;
        }

        /**
         * @param expected 
         * @param desired 
         * @param order 
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool compare_exchange_strong(T& expected, T desired, MemoryOrder order = DefaultMemoryOrder) volatile noexcept
        {
            if (order == memory_order_acq_rel || order == memory_order_release)
                return __atomic_compare_exchange(&m_value, &expected, &desired, false, memory_order_release, memory_order_acquire);
            return __atomic_compare_exchange(&m_value, &expected, &desired, false, order, order);
        }

        /**
         * @return T 
         */
        ALWAYS_INLINE operator T() const volatile noexcept
        {
            return load();
        }

        /**
         * @param order 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T load(MemoryOrder order = DefaultMemoryOrder) const volatile noexcept
        {
            alignas(T) u8 buffer[sizeof(T)];
            T* ret = reinterpret_cast<T*>(buffer);
            __atomic_load(&m_value, ret, order);
            return *ret;
        }

        /**
         * @param desired 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T operator=(T desired) volatile noexcept
        {
            store(desired);
            return desired;
        }

        /**
         * @param desired 
         * @param order 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void store(T desired, MemoryOrder order = DefaultMemoryOrder) volatile noexcept
        {
            __atomic_store(&m_value, &desired, order);
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE bool is_lock_free() const volatile noexcept
        {
            return __atomic_is_lock_free(sizeof(m_value), &m_value);
        }
    }; // class Atomic
    
    /**
     * @tparam T 
     * @tparam DefaultMemoryOrder 
     */
    template<Integral T, MemoryOrder DefaultMemoryOrder>
    class Atomic<T, DefaultMemoryOrder> 
    {
        T m_value { 0 };

    public:
        /**
         * @brief Construct a new Atomic object
         * 
         */
        Atomic() noexcept = default;

        /**
         * @return Atomic& 
         */
        Atomic& operator=(const Atomic&) volatile = delete;

        /**
         * @return Atomic& 
         */
        Atomic& operator=(Atomic&&) volatile = delete;

        /**
         * @brief Construct a new Atomic object
         * 
         */
        Atomic(const Atomic&) = delete;

        /**
         * @brief Construct a new Atomic object
         * 
         */
        Atomic(Atomic&&) = delete;

        constexpr Atomic(T val) noexcept
            : m_value(val)
        { }

        /**
         * @return volatile* 
         */
        volatile T* ptr() noexcept
        {
            return &m_value;
        }

        /**
         * @param desired 
         * @param order 
         * @return T 
         */
        T exchange(T desired, MemoryOrder order = DefaultMemoryOrder) volatile noexcept
        {
            return __atomic_exchange_n(&m_value, desired, order);
        }

        /**
         * @param expected 
         * @param desired 
         * @param order 
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool compare_exchange_strong(T& expected, T desired, MemoryOrder order = DefaultMemoryOrder) volatile noexcept
        {
            if (order == memory_order_acq_rel || order == memory_order_release)
                return __atomic_compare_exchange_n(&m_value, &expected, desired, false, memory_order_release, memory_order_acquire);
            return __atomic_compare_exchange_n(&m_value, &expected, desired, false, order, order);
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T operator++() volatile noexcept
        {
            return fetch_add(1) + 1;
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T operator++(int) volatile noexcept
        {
            return fetch_add(1);
        }

        /**
         * @param val 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T operator+=(T val) volatile noexcept
        {
            return fetch_add(val) + val;
        }

        /**
         * @param val 
         * @param order 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T fetch_add(T val, MemoryOrder order = DefaultMemoryOrder) volatile noexcept
        {
            return __atomic_fetch_add(&m_value, val, order);
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T operator--() volatile noexcept
        {
            return fetch_sub(1) - 1;
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T operator--(int) volatile noexcept
        {
            return fetch_sub(1);
        }

        ALWAYS_INLINE T operator-=(T val) volatile noexcept
        {
            return fetch_sub(val) - val;
        }

        /**
         * @param val 
         * @param order 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T fetch_sub(T val, MemoryOrder order = DefaultMemoryOrder) volatile noexcept
        {
            return __atomic_fetch_sub(&m_value, val, order);
        }

        /**
         * @param val 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T operator&=(T val) volatile noexcept
        {
            return fetch_and(val) & val;
        }

        /**
         * @param val 
         * @param order 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T fetch_and(T val, MemoryOrder order = DefaultMemoryOrder) volatile noexcept
        {
            return __atomic_fetch_and(&m_value, val, order);
        }

        /**
         * @param val 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T operator|=(T val) volatile noexcept
        {
            return fetch_or(val) | val;
        }

        /**
         * @param val 
         * @param order 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T fetch_or(T val, MemoryOrder order = DefaultMemoryOrder) volatile noexcept
        {
            return __atomic_fetch_or(&m_value, val, order);
        }

        /**
         * @param val 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T operator^=(T val) volatile noexcept
        {
            return fetch_xor(val) ^ val;
        }

        /**
         * @param val 
         * @param order 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T fetch_xor(T val, MemoryOrder order = DefaultMemoryOrder) volatile noexcept
        {
            return __atomic_fetch_xor(&m_value, val, order);
        }
        
        /**
         * @return T 
         */
        ALWAYS_INLINE operator T() const volatile noexcept
        {
            return load();
        }

        /**
         * @param order 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T load(MemoryOrder order = DefaultMemoryOrder) const volatile noexcept
        {
            return __atomic_load_n(&m_value, order);
        }

        /**
         * @param desired 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T operator=(T desired) volatile noexcept
        {
            store(desired);
            return desired;
        }

        /**
         * @param desired 
         * @param order 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void store(T desired, MemoryOrder order = DefaultMemoryOrder) volatile noexcept
        {
            __atomic_store_n(&m_value, desired, order);
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE bool is_lock_free() const volatile noexcept
        {
            return __atomic_is_lock_free(sizeof(m_value), &m_value);
        }
    }; // class Atomic<T, DefaultMemoryOrder> 

    /**
     * @tparam T 
     * @tparam DefaultMemoryOrder 
     */
    template<typename T, MemoryOrder DefaultMemoryOrder>
    class Atomic<T*, DefaultMemoryOrder> 
    {
        T* m_value { nullptr };

    public:
        /**
         * @brief Construct a new Atomic object
         * 
         */
        Atomic() noexcept = default;

        /**
         * @return Atomic& 
         */
        Atomic& operator=(const Atomic&) volatile = delete;

        /**
         * @return Atomic& 
         */
        Atomic& operator=(Atomic&&) volatile = delete;

        /**
         * @brief Construct a new Atomic object
         * 
         */
        Atomic(const Atomic&) = delete;

        /**
         * @brief Construct a new Atomic object
         * 
         */
        Atomic(Atomic&&) = delete;

        constexpr Atomic(T* val) noexcept
            : m_value(val)
        { }

        /**
         * @return volatile** 
         */
        volatile T** ptr() noexcept
        {
            return &m_value;
        }
        
        /**
         * @param desired 
         * @param order 
         * @return T* 
         */
        T* exchange(T* desired, MemoryOrder order = DefaultMemoryOrder) volatile noexcept
        {
            return __atomic_exchange_n(&m_value, desired, order);
        }

        /**
         * @param expected 
         * @param desired 
         * @param order 
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool compare_exchange_strong(T*& expected, T* desired, MemoryOrder order = DefaultMemoryOrder) volatile noexcept
        {
            if (order == memory_order_acq_rel || order == memory_order_release)
                return __atomic_compare_exchange_n(&m_value, &expected, desired, false, memory_order_release, memory_order_acquire);
            return __atomic_compare_exchange_n(&m_value, &expected, desired, false, order, order);
        }

        T* operator++() volatile noexcept
        {
            return fetch_add(1) + 1;
        }

        T* operator++(int) volatile noexcept
        {
            return fetch_add(1);
        }

        /**
         * @param val 
         * @return T* 
         */
        T* operator+=(ptrdiff_t val) volatile noexcept
        {
            return fetch_add(val) + val;
        }

        /**
         * @param val 
         * @param order 
         * @return T* 
         */
        T* fetch_add(ptrdiff_t val, MemoryOrder order = DefaultMemoryOrder) volatile noexcept
        {
            return __atomic_fetch_add(&m_value, val * sizeof(*m_value), order);
        }

        T* operator--() volatile noexcept
        {
            return fetch_sub(1) - 1;
        }

        T* operator--(int) volatile noexcept
        {
            return fetch_sub(1);
        }

        /**
         * @param val 
         * @return T* 
         */
        T* operator-=(ptrdiff_t val) volatile noexcept
        {
            return fetch_sub(val) - val;
        }

        /**
         * @param val 
         * @param order 
         * @return T* 
         */
        T* fetch_sub(ptrdiff_t val, MemoryOrder order = DefaultMemoryOrder) volatile noexcept
        {
            return __atomic_fetch_sub(&m_value, val * sizeof(*m_value), order);
        }

        /**
         * @return T* 
         */
        operator T*() const volatile noexcept
        {
            return load();
        }

        /**
         * @param order 
         * @return T* 
         */
        T* load(MemoryOrder order = DefaultMemoryOrder) const volatile noexcept
        {
            return __atomic_load_n(&m_value, order);
        }

        /**
         * @param desired 
         * @return T* 
         */
        T* operator=(T* desired) volatile noexcept
        {
            store(desired);
            return desired;
        }

        /**
         * @param desired 
         * @param order 
         */
        void store(T* desired, MemoryOrder order = DefaultMemoryOrder) volatile noexcept
        {
            __atomic_store_n(&m_value, desired, order);
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_lock_free() const volatile noexcept
        {
            return __atomic_is_lock_free(sizeof(m_value), &m_value);
        }
    }; // class Atomic<T*, DefaultMemoryOrder> 

} // namespace Mods

using Mods::Atomic;
using Mods::full_memory_barrier;
