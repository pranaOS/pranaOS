/**
 * @file fenv.cpp
 * @author Krisna Pranav
 * @brief fenv
 * @version 6.0
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/types.h>
#include <fenv.h>

static_assert(sizeof(__x87_floating_point_environment) == 28);
