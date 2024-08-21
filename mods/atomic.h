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

#include "platform.h"
#include "types.h"

namespace Mods {

    enum MemoryOrder {
        memory_order_relaxed = __ATOMIC_RELAXED,
        memory_order_consume = __ATOMIC_CONSUME,
        memory_order_acquire = __ATOMIC_ACQUIRE,
        memory_order_release = __ATOMIC_RELEASE,
        memory_order_acq_rel = __ATOMIC_ACQ_REL,
        memory_order_seq_cst = __ATOMIC_SEQ_CST
    };

    /**
     * @brief atomc_exchange
     * 
     * @tparam T 
     * @param var 
     * @param desired 
     * @param order 
     * @return T 
     */
    template<typename T>
    static inline T atomic_exchange(volatile T* var, T desired, MemoryOrder order = memory_order_seq_cst) noexcept {
        return __atomic_exchange_n(var, desired, order);
    }

    /**
     * @tparam T 
     * @tparam RemoveVolatile<T>::Type 
     * @param var 
     * @param desired 
     * @param order 
     * @return V* 
     */
    template<typename T, typename V = typename RemoveVolatile<T>::Type>
    static inline V* atomic_exchange(volatile T** var, V* desired, MemoryOrder order = memory_order_seq_cst) noexcept {
        return __atomic_exchange_n(var, desired, order);
    }

    /**
     * @tparam T 
     * @tparam RemoveVolatile<T>::Type 
     * @param var 
     * @param order 
     * @return V* 
     */
    template<typename T, typename V = typename RemoveVolatile<T>::Type>
    static inline V* atomic_exchange(volatile T** var, std::nullptr_t, MemoryOrder order = memory_order_seq_cst) noexcept {
        return __atomic_exchange_n(const_cast<V**>(var), nullptr, order);
    }

    /**
     * @brief atomc_compare_exchange_strong
     * 
     * @tparam T 
     * @param var 
     * @param expected 
     * @param desired 
     * @param order 
     * @return true 
     * @return false 
     */
    template<typename T>
    [[nodiscard]] static inline bool atomic_compare_exchange_strong(volatile T* var, T& expected, T desired, MemoryOrder order = memory_order_seq_cst) noexcept {
        if (order == memory_order_acq_rel || order == memory_order_release)
            return __atomic_compare_exchange_n(var, &expected, desired, false, memory_order_release, memory_order_acquire);
        else
            return __atomic_compare_exchange_n(var, &expected, desired, false, order, order);
    }

    /**
     * @tparam T 
     * @tparam RemoveVolatile<T>::Type 
     * @param var 
     * @param expected 
     * @param desired 
     * @param order 
     * @return true 
     * @return false 
     */
    template<typename T, typename V = typename RemoveVolatile<T>::Type>
    [[nodiscard]] static inline bool atomic_compare_exchange_strong(volatile T** var, V*& expected, V* desired, MemoryOrder order = memory_order_seq_cst) noexcept {
        if (order == memory_order_acq_rel || order == memory_order_release)
            return __atomic_compare_exchange_n(var, &expected, desired, false, memory_order_release, memory_order_acquire);
        else
            return __atomic_compare_exchange_n(var, &expected, desired, false, order, order);
    }

    /**
     * @tparam T 
     * @tparam RemoveVolatile<T>::Type 
     * @param var 
     * @param expected 
     * @param order 
     * @return true 
     * @return false 
     */
    template<typename T, typename V = typename RemoveVolatile<T>::Type>
    [[nodiscard]] static inline bool atomic_compare_exchange_strong(volatile T** var, V*& expected, std::nullptr_t, MemoryOrder order = memory_order_seq_cst) noexcept {
        if (order == memory_order_acq_rel || order == memory_order_release)
            return __atomic_compare_exchange_n(const_cast<V**>(var), &expected, nullptr, false, memory_order_release, memory_order_acquire);
        else
            return __atomic_compare_exchange_n(const_cast<V**>(var), &expected, nullptr, false, order, order);
    }

    /**
     * @brief atomic_fetch_add
     * 
     * @tparam T 
     * @param var 
     * @param val 
     * @param order 
     * @return T 
     */
    template<typename T>
    static inline T atomic_fetch_add(volatile T* var, T val, MemoryOrder order = memory_order_seq_cst) noexcept {
        return __atomic_fetch_add(var, val, order);
    }

    /**
     * @brief atomic_fetch_sub
     * 
     * @tparam T 
     * @param var 
     * @param val 
     * @param order 
     * @return T 
     */
    template<typename T>
    static inline T atomic_fetch_sub(volatile T* var, T val, MemoryOrder order = memory_order_seq_cst) noexcept {
        return __atomic_fetch_sub(var, val, order);
    }

    /**
     * @brief atomic_fetch_and
     * 
     * @tparam T 
     * @param var 
     * @param val 
     * @param order 
     * @return T 
     */
    template<typename T>
    static inline T atomic_fetch_and(volatile T* var, T val, MemoryOrder order = memory_order_seq_cst) noexcept {
        return __atomic_fetch_and(var, val, order);
    }  

    /**
     * @brief atomic_fetch_or
     * 
     * @tparam T 
     * @param var 
     * @param val 
     * @param order 
     * @return T 
     */
    template<typename T>
    static inline T atomic_fetch_or(volatile T* var, T val, MemoryOrder order = memory_order_seq_cst) noexcept {
        return __atomic_fetch_or(var, val, order);
    }

    /**
     * @brief atomic_fetch_xor
     * 
     * @tparam T 
     * @param var 
     * @param val 
     * @param order 
     * @return T 
     */
    template<typename T>
    static inline T atomic_fetch_xor(volatile T* var, T val, MemoryOrder order = memory_order_seq_cst) noexcept {
        return __atomic_fetch_xor(var, val, order);
    }

    /**
     * @brief atomic_load
     * 
     * @tparam T 
     * @param var 
     * @param order 
     * @return T 
     */
    template<typename T>
    static inline T atomic_load(volatile T* var, MemoryOrder order = memory_order_seq_cst) noexcept {
        return __atomic_load_n(var, order);
    }

    /**
     * @tparam T 
     * @tparam RemoveVolatile<T>::Type 
     * @param var 
     * @param order 
     * @return V* 
     */
    template<typename T, typename V = typename RemoveVolatile<T>::Type>
    static inline V* atomic_load(volatile T** var, MemoryOrder order = memory_order_seq_cst) noexcept {
        return __atomic_load_n(const_cast<V**>(var), order);
    }

    /**
     * @tparam T 
     * @param var 
     * @param desired 
     * @param order 
     */
    template<typename T>
    static inline void atomic_store(volatile T* var, T desired, MemoryOrder order = memory_order_seq_cst) noexcept {
        __atomic_store_n(var, desired, order);
    }

    /**
     * @tparam T 
     * @tparam RemoveVolatile<T>::Type 
     * @param var 
     * @param desired 
     * @param order 
     */
    template<typename T, typename V = typename RemoveVolatile<T>::Type>
    static inline void atomic_store(volatile T** var, V* desired, MemoryOrder order = memory_order_seq_cst) noexcept {
        __atomic_store_n(var, desired, order);
    }

    /**
     * @tparam T 
     * @tparam RemoveVolatile<T>::Type 
     * @param var 
     * @param order 
     */
    template<typename T, typename V = typename RemoveVolatile<T>::Type>
    static inline void atomic_store(volatile T** var, std::nullptr_t, MemoryOrder order = memory_order_seq_cst) noexcept {
        __atomic_store_n(const_cast<V**>(var), nullptr, order);
    }

    template<typename T>
    class Atomic {
        T m_value { 0 };

    public:
        Atomic() noexcept = default;
        Atomic(const Atomic&) = delete;
        Atomic& operator=(const Atomic&) volatile = delete;

        Atomic(T val) noexcept
            : m_value(val)
        {
        }

        /**
         * @brief ptr
         * 
         * @return volatile* 
         */
        volatile T* ptr() noexcept {
            return &m_value;
        }

        /**
         * @brief exchange memory orders 
         * 
         * @param desired 
         * @param order 
         * @return T 
         */
        T exchange(T desired, MemoryOrder order = memory_order_seq_cst) volatile noexcept {
            return __atomic_exchange_n(&m_value, desired, order);
        }

        /**
         * @param expected 
         * @param desired 
         * @param order 
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool compare_exchange_strong(T& expected, T desired, MemoryOrder order = memory_order_seq_cst) volatile noexcept {
            if (order == memory_order_acq_rel || order == memory_order_release)
                return __atomic_compare_exchange_n(&m_value, &expected, desired, false, memory_order_release, memory_order_acquire);
            else
                return __atomic_compare_exchange_n(&m_value, &expected, desired, false, order, order);
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T operator++() volatile noexcept {
            return fetch_add(1) + 1;
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T operator++(int) volatile noexcept {
            return fetch_add(1);
        }

        /**
         * @param val 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T operator+=(T val) volatile noexcept {
            return fetch_add(val) + val;
        }

        /**
         * @param val 
         * @param order 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T fetch_add(T val, MemoryOrder order = memory_order_seq_cst) volatile noexcept {
            return __atomic_fetch_add(&m_value, val, order);
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T operator--() volatile noexcept {
            return fetch_sub(1) - 1;
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T operator--(int) volatile noexcept {
            return fetch_sub(1);
        }

        /**
         * @param val 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T operator-=(T val) volatile noexcept {
            return fetch_sub(val) - val;
        }

        /**
         * @brief fetch_sub 
         * 
         * @param val 
         * @param order 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T fetch_sub(T val, MemoryOrder order = memory_order_seq_cst) volatile noexcept {
            return __atomic_fetch_sub(&m_value, val, order);
        }

        /**
         * @param val 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T operator&=(T val) volatile noexcept {
            return fetch_and(val) & val;
        }

        /**
         * @brief and
         * 
         * @param val 
         * @param order 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T fetch_and(T val, MemoryOrder order = memory_order_seq_cst) volatile noexcept {
            return __atomic_fetch_and(&m_value, val, order);
        }

        /**
         * @param val 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T operator|=(T val) volatile noexcept {
            return fetch_or(val) | val;
        }

        /**
         * @brief fetch_or
         * 
         * @param val 
         * @param order 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T fetch_or(T val, MemoryOrder order = memory_order_seq_cst) volatile noexcept {
            return __atomic_fetch_or(&m_value, val, order);
        }

        /**
         * @param val 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T operator^=(T val) volatile noexcept {
            return fetch_xor(val) ^ val;
        }
        
        /**
         * @brief xor
         * 
         * @param val 
         * @param order 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T fetch_xor(T val, MemoryOrder order = memory_order_seq_cst) volatile noexcept {
            return __atomic_fetch_xor(&m_value, val, order);
        }
        
        /**
         * @return T 
         */
        ALWAYS_INLINE operator T() const volatile noexcept {
            return load();
        }

        /**
         * @brief load
         * 
         * @param order 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T load(MemoryOrder order = memory_order_seq_cst) const volatile noexcept {
            return __atomic_load_n(&m_value, order);
        }

        /**
         * @param desired 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T operator=(T desired) volatile noexcept {
            store(desired);
            return desired;
        }

        /**
         * @brief store
         * 
         * @param desired 
         * @param order 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void store(T desired, MemoryOrder order = memory_order_seq_cst) volatile noexcept {
            __atomic_store_n(&m_value, desired, order);
        }

        /**
         * @brief is_lock_free
         * 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE bool is_lock_free() const volatile noexcept {
            return __atomic_is_lock_free(sizeof(m_value), &m_value);
        }
    };

    template<typename T>
    class Atomic<T*> {
        T* m_value { nullptr };

    public:
        Atomic() noexcept = default;
        Atomic(const Atomic&) = delete;
        Atomic& operator=(const Atomic&) volatile = delete;

        Atomic(T* val) noexcept
            : m_value(val)
        {
        }

        /**
         * @brief ptr
         * 
         * @return volatile** 
         */
        volatile T** ptr() noexcept {
            return &m_value;
        }

        /**
         * @param desired 
         * @param order 
         * @return T* 
         */
        T* exchange(T* desired, MemoryOrder order = memory_order_seq_cst) volatile noexcept {
            return __atomic_exchange_n(&m_value, desired, order);
        }

        /**
         * @param expected 
         * @param desired 
         * @param order 
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool compare_exchange_strong(T*& expected, T* desired, MemoryOrder order = memory_order_seq_cst) volatile noexcept {
            if (order == memory_order_acq_rel || order == memory_order_release)
                return __atomic_compare_exchange_n(&m_value, &expected, desired, false, memory_order_release, memory_order_acquire);
            else
                return __atomic_compare_exchange_n(&m_value, &expected, desired, false, order, order);
        }

        /**
         * @return T* 
         */
        T* operator++() volatile noexcept {
            return fetch_add(1) + 1;
        }

        /**
         * @return T* 
         */
        T* operator++(int) volatile noexcept {
            return fetch_add(1);
        }

        /**
         * @param val 
         * @return T* 
         */
        T* operator+=(ptrdiff_t val) volatile noexcept {
            return fetch_add(val) + val;
        }

        /**
         * @brief fetch_add
         * 
         * @param val 
         * @param order 
         * @return T* 
         */
        T* fetch_add(ptrdiff_t val, MemoryOrder order = memory_order_seq_cst) volatile noexcept {
            return __atomic_fetch_add(&m_value, val * sizeof(*m_value), order);
        }

        /**
         * @return T* 
         */
        T* operator--() volatile noexcept {
            return fetch_sub(1) - 1;
        }

        /**
         * @return T* 
         */
        T* operator--(int) volatile noexcept {
            return fetch_sub(1);
        }

        /**
         * @param val 
         * @return T* 
         */
        T* operator-=(ptrdiff_t val) volatile noexcept {
            return fetch_sub(val) - val;
        }

        /**
         * @brief fetch_sub
         * 
         * @param val 
         * @param order 
         * @return T* 
         */
        T* fetch_sub(ptrdiff_t val, MemoryOrder order = memory_order_seq_cst) volatile noexcept {
            return __atomic_fetch_sub(&m_value, val * sizeof(*m_value), order);
        }

        /**
         * @return T* 
         */
        operator T*() const volatile noexcept {
            return load();
        }

        /**
         * @brief load
         * 
         * @param order 
         * @return T* 
         */
        T* load(MemoryOrder order = memory_order_seq_cst) const volatile noexcept {
            return __atomic_load_n(&m_value, order);
        }

        /**
         * @param desired 
         * @return T* 
         */
        T* operator=(T* desired) volatile noexcept {
            store(desired);
            return desired;
        }

        /**
         * @brief store
         * 
         * @param desired 
         * @param order 
         */
        void store(T* desired, MemoryOrder order = memory_order_seq_cst) volatile noexcept {
            __atomic_store_n(&m_value, desired, order);
        }

        /**
         * @brief is_lock_free
         * 
         * @return true 
         * @return false 
         */
        bool is_lock_free() const volatile noexcept {
            return __atomic_is_lock_free(sizeof(m_value), &m_value);
        }
    };

}

using Mods::Atomic;
