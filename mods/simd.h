/**
 * @file simd.h
 * @author Krisna Pranav
 * @brief SIMD
 * @version 1.0
 * @date 2023-07-25
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 


#include "types.h"

namespace Mods::SIMD
{
    using i8x2 = i8 __attribute__((vector_size(2)));
}