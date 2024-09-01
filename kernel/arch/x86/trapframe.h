/**
 * @file trapframe.h
 * @author Krisna Pranav
 * @brief trap frame
 * @version 6.0
 * @date 2024-09-01
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

    struct RegisterState;

    struct TrapFrame 
    {
        FlatPtr prev_irq_level;

        TrapFrame* next_trap;

        RegisterState* regs; 

        /**
         * @brief Construct a new Trap Frame object
         * 
         */
        TrapFrame() = delete;

        /**
         * @brief Construct a new Trap Frame object
         * 
         */
        TrapFrame(const TrapFrame&) = delete;

        TrapFrame(TrapFrame&&) = delete;

        TrapFrame& operator=(const TrapFrame&) = delete;

        TrapFrame& operator=(TrapFrame&&) = delete;
    }; // struct TrapFrame

    #if ARCH(I386)
    #    define TRAP_FRAME_SIZE (3 * 4)
    #else
    #    define TRAP_FRAME_SIZE (3 * 8)
    #endif
    
    static_assert(AssertSize<TrapFrame, TRAP_FRAME_SIZE>());

    extern "C" void enter_trap_no_irq(TrapFrame* trap) __attribute__((used));

    extern "C" void enter_trap(TrapFrame*) __attribute__((used));

    extern "C" void exit_trap(TrapFrame*) __attribute__((used));
    
} // namespace Kernel
