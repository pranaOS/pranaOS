/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <base/Concepts.h>
#include <base/Platform.h>
#include <base/Types.h>

namespace Base
{

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

}