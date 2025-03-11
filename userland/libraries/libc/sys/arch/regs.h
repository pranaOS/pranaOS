/**
 * @file regs.h
 * @author Krisna Pranav
 * @brief regs
 * @version 0.1
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/platform.h>

#if ARCH(I386)
#    include "i386/regs.h"
#elif ARCH(X86_64)
#    include "x86_64/regs.h"
#endif
