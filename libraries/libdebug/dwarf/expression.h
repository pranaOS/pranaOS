/**
 * @file expression.h
 * @author Krisna Pranav
 * @brief expression
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "mods/bytebuffer.h"
#include "mods/types.h"

struct PtraceRegisters;

enum class Type
{
    None,
    UnsignedInteger,
    Register,
}; // enum class Type

struct  Value 
{
    Type type;

    union {
        FlatPtr as_addr;
        u32 as_u32;
    } data { 0 };
}; // struct  Value 
