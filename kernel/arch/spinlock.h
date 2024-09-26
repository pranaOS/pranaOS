/**
 * @file spinlock.h
 * @author Krisna Pranav
 * @brief Spin Lock
 * @version 6.0
 * @date 2024-09-26
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/platform.h>

#if ARCH(X86_64) || ARCH(I386)
#    include <kernel/arch/x86/spinlock.h>
#elif ARCH(AARCH64)
#    include <kernel/arch/aarch64/spinlock.h>
#else
#    error "Unknown architecture"
#endif
