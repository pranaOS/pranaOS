/**
 * @file regs.h
 * @author Krisna Pranav
 * @brief registers 
 * @version 6.0
 * @date 2023-07-05
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>

/**
 * @brief PTraceRegisters
 * 
 */
struct [[gnu::packed]] PtraceRegisters {
    u32 eax;
    u32 ecx;
    u32 edx;
    u32 ebx;
    u32 esp;
    u32 ebp;
    u32 esi;
    u32 edi;
    u32 eip;
    u32 eflags;
    u32 cs;
    u32 ss;
    u32 ds;
    u32 es;
    u32 fs;
    u32 gs;
};
