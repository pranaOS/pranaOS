/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <base/Concepts.h>
#include <base/Platform.h>
#include <base/Types.h>

namespace Base {

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
atomic_signal_fence(Base::MemoryOrder::memory_order_acq_rel);
atomic_thread_fence(Base::MemoryOrder::memory_order_acq_rel);
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

}