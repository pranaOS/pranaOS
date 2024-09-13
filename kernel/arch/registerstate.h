/**
 * @file registerstate.h
 * @author Krisna Pranav
 * @brief Register State
 * @version 6.0
 * @date 2024-09-13
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/platform.h>

#if ARCH(X86_64) || ARCH(I386)
#    include <kernel/arch/x86/registerstate.h>
#elif ARCH(AARCH64)
#    include <kernel/arch/aarch64/registerstate.h>
#else
#    error "Unknown architecture"
#endif
