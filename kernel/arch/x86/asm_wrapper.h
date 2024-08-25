/**
 * @file asm_wrapper.h
 * @author Krisna Pranav
 * @brief asm wrapper
 * @version 6.0
 * @date 2024-08-25
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>
#include <mods/platform.h>

VALIDATE_IS_X86()

namespace Kernel
{
    ALWAYS_INLINE void cli()
    {

    }

    FlatPtr read_cr0();
    FlatPtr read_cr2();
    FlatPtr read_cr3();
    FlatPtr read_cr4();
    u64 read_xcr0();

    void write_cr0(FlatPtr);
    void write_cr3(FlatPtr);
    void write_cr4(FlatPtr);
    void write_xcr0(u64);
} // namespace Kernel