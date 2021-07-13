/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libutils/Prelude.h>

extern "C" void arch_x86_32_main(void *info, uint32_t magic);

extern "C" void _kstart_stivale2(void *info)
{
    arch_x86_32_main(info, 0x73747632);
}