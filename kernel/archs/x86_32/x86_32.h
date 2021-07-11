/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <libutils/Prelude.h>
#include "archs/x86/IOPort.h"
#include "archs/x86/x86.h"

namespace Arch::x86_32
{

static inline uint32_t EBP()
{
    uint32_t r;
    asm volatile("mov %%ebp, %0"
                 : "=r"(r));
    return r;
}

static inline uint32_t ESP()
{
    uint32_t r;
    asm volatile("mov %%esp, %0"
                 : "=r"(r));
    return r;
}


}