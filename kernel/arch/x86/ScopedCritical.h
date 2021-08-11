/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <base/Types.h>
#include <kernel/arch/x86/Processor.h>

namespace Kernel {

class ScopedCritical {
    BASE_MAKE_NONCOPYABLE(ScopedCritical);

public:
    ScopedCritical()
    {
        enter();
    }

    ~ScopedCritical()
    {
        if (m_valid)
            leave();
    }

    ScopedCritical(ScopedCritical&& from)
        : m_valid(exchange(from.m_valid, false))
    {
    }

    ScopedCritical& operator=(ScopedCritical&& from)
    {
        if (&from != this) {
            m_valid = exchange(from.m_valid, false);
        }
        return *this;
    }

    void leave()
    {
        VERIFY(m_valid);
        m_valid = false;
        Processor::leave_critical();
    }

    void enter()
    {
        VERIFY(!m_valid);
        m_valid = true;
        Processor::enter_critical();
    }

private:
    bool m_valid { false };
};

}
