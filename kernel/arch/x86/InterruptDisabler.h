/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Types.h>
#include <kernel/IO.h>
#include <kernel/arch/x86/ASM_wrapper.h>

namespace Kernel {

class InterruptDisabler {
public:
InterruptDisabler()
    {
        m_flags = cpu_Flags();

cli();
    }

~InterruptDisabler()
    {
if (m_flags & 0x200)
sti();
    }

private:
    u32 m_flags;
};

}