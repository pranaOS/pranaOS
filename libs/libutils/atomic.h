/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libutils/concepts.h>
#include <libutils/platform.h>
#include <libutils/types.h>

namespace Utils {

static inline void atomic_signal_fence(MemoryOrder order) noexcept
{
    return __atomic_signal_fence(order);
}

static inline void atomic_thread_fence(MemoryOrder order) noexcept
{
    return __atomic_thread_fence(order);
}

static inline void full_memory_barrier() noexcept
{
    atomic_signal_fence(Utils::MemoryOrder::memory_order_acq_rel);
    atomic_thread_fence(Utils::MemoryOrder::memory_order_acq_rel);
}

template<typename T>
static inline T atomic_exchange(volatile T* var, T desired, MemoryOrder order = memory_order_seq_cst) noexcept
{
    return __atomic_exchange_n(var, desired, order);
}

template<typename T, typename V = RemoveVolatile<T>>
static inline V* atomic_exchange(volatile T** var, V* desired, MemoryOrder order = memory_order_seq_cst) noexcept
{
    return __atomic_exchange_n(var, desired, order);
}

template<typename T, typename V = RemoveVolatile<T>>
static inline V* atomic_exchange(volatile T** var, std::nullptr_t, MemoryOrder order = memory_order_seq_cst) noexcept
{
    return __atomic_exchange_n(const_cast<V**>(var), nullptr, order);
}

template<typename T>
[[nodiscard]] static inline bool atomic_compare_exchange_strong(volatile T* var, T& expected, T desired, MemoryOrder order = memory_order_seq_cst) noexcept
{
    if (order == memory_order_acq_rel || order == memory_order_release)
        return __atomic_compare_exchange_n(var, &expected, desired, false, memory_order_release, memory_order_acquire);
    else
        return __atomic_compare_exchange_n(var, &expected, desired, false, order, order);
}

template<typename T, typename V = RemoveVolatile<T>>
[[nodiscard]] static inline bool atomic_compare_exchange_strong(volatile T** var, V*& expected, V* desired, MemoryOrder order = memory_order_seq_cst) noexcept
{
    if (order == memory_order_acq_rel || order == memory_order_release)
        return __atomic_compare_exchange_n(var, &expected, desired, false, memory_order_release, memory_order_acquire);
    else
        return __atomic_compare_exchange_n(var, &expected, desired, false, order, order);
}

template<typename T, typename V = RemoveVolatile<T>>
[[nodiscard]] static inline bool atomic_compare_exchange_strong(volatile T** var, V*& expected, std::nullptr_t, MemoryOrder order = memory_order_seq_cst) noexcept
{
    if (order == memory_order_acq_rel || order == memory_order_release)
        return __atomic_compare_exchange_n(const_cast<V**>(var), &expected, nullptr, false, memory_order_release, memory_order_acquire);
    else
        return __atomic_compare_exchange_n(const_cast<V**>(var), &expected, nullptr, false, order, order);
}

template<typename T>
static inline T atomic_fetch_add(volatile T* var, T val, MemoryOrder order = memory_order_seq_cst) noexcept
{
    return __atomic_fetch_add(var, val, order);
}

template<typename T>
static inline T atomic_fetch_sub(volatile T* var, T val, MemoryOrder order = memory_order_seq_cst) noexcept
{
    return __atomic_fetch_sub(var, val, order);
}

template<typename T>
static inline T atomic_fetch_and(volatile T* var, T val, MemoryOrder order = memory_order_seq_cst) noexcept
{
    return __atomic_fetch_and(var, val, order);
}

template<typename T>
static inline T atomic_fetch_or(volatile T* var, T val, MemoryOrder order = memory_order_seq_cst) noexcept
{
    return __atomic_fetch_or(var, val, order);
}

template<typename T>
static inline T atomic_fetch_xor(volatile T* var, T val, MemoryOrder order = memory_order_seq_cst) noexcept
{
    return __atomic_fetch_xor(var, val, order);
}

template<typename T>
static inline T atomic_load(volatile T* var, MemoryOrder order = memory_order_seq_cst) noexcept
{
    return __atomic_load_n(var, order);
}

template<typename T, typename V = RemoveVolatile<T>>
static inline V* atomic_load(volatile T** var, MemoryOrder order = memory_order_seq_cst) noexcept
{
    return __atomic_load_n(const_cast<V**>(var), order);
}

template<typename T>
static inline void atomic_store(volatile T* var, T desired, MemoryOrder order = memory_order_seq_cst) noexcept
{
    __atomic_store_n(var, desired, order);
}

template<typename T, typename V = RemoveVolatile<T>>
static inline void atomic_store(volatile T** var, V* desired, MemoryOrder order = memory_order_seq_cst) noexcept
{
    __atomic_store_n(var, desired, order);
}

template<typename T, typename V = RemoveVolatile<T>>
static inline void atomic_store(volatile T** var, std::nullptr_t, MemoryOrder order = memory_order_seq_cst) noexcept
{
    __atomic_store_n(const_cast<V**>(var), nullptr, order);
}

template<typename T>
static inline bool atomic_is_lock_free(volatile T* ptr = nullptr) noexcept
{
    return __atomic_is_lock_free(sizeof(T), ptr);
}

}