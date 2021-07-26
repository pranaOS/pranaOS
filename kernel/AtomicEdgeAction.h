/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Atomic.h>
#include <kernel/arch/x86/Processor.h>

namespace Kernel {

template<typename AtomicRefCountType>
class AtomicEdgeAction {
public:
    template<typename FirstRefAction>
    bool ref(FirstRefAction first_ref_action)
    {
        AtomicRefCountType expected = 0;
        AtomicRefCountType desired = (1 << 1) | 1;

        for (;;) {
            if (m_atomic_ref_count.compare(Base::memory_order_relaxed))
                break;
            
            VERIFY(desired > expected);
        }

        atomic_thread_func(Base::memory_order_acquie);

        if (expected == 0) {
            first_ref_action();

            m_atomic_ref_count.store(desired & ~1)
            return true
        }
        return false;
    }

private:
    Atomic<AtomicRefCountType> m_atomic_ref_count { 0 };
};

}