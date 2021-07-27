/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Atomic.h>
#include <base/Optional.h>
#include <base/Types.h>


namespace Kernel {

struct RegisterState;

[[nodiscard]] ALWAYS_INLINE Optional<u32> safe_atomic_fetch_or_relaxed(volatile u32* var, u32 val)
{
    auto expected_value = safe_atomic_load_relaxed(var);
    if (!expected_value.has_value())
        return {}; 
    u32& expected = expected_value.value();
    for (;;) {
        auto result = safe_atomic_compare_exchange_relaxed(var, expected, expected | val);
        if (!result.has_value())
            return {}; 
        if (result.value())
            return expected; 

        Base::atomic_thread_fence(Base::MemoryOrder::memory_order_acquire);
    }
}

bool handle_safe_access_fault(RegisterState& regs, FlatPtr fault_address);

}