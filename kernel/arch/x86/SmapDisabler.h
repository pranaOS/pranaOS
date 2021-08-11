/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <kernel/arch/x86/ASM_wrapper.h>

namespace Kernel {

class SmapDisabler {
public:
    ALWAYS_INLINE SmapDisabler()
        : m_flags(cpu_flags())
    {
        stac();
    }

    ALWAYS_INLINE ~SmapDisabler()
    {
        if (!(m_flags & 0x40000))
            clac();
    }

private:
    const FlatPtr m_flags;
};

}
