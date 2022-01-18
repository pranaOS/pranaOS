//
// Created by Krisna Pranav on 18/01/22.
//

#pragma once

#include "process.h"
#include <ak/list.h>
#include <ak/types.h>
#include <cpu/register.h>

namespace Kernel {
    #define THREAD_STACK_SIZE 4_KB

    #define SEG_USER_DATA 0x23
    #define SEG_USER_CODE 0x1B
    #define SEG_KERNEL_DATA 0x10
    #define SEG_KERNEL_CODE 8

    enum threadState {
        Blocked,
        Ready,
        Stopped,
        Started
    };

    enum blockedState {
        Unknown,
        Sleep,
        ReceiveIPC
    };

    struct process;

    struct thread {
        Process* parent;
        ak::uint8_t* stack;
        ak::uint8_t userStack;
        ak::uint32_t userStackSize;
        threadState state;
        blockedState blockedstate;
        Kernel::CPUState* regsPtr;

        ak::uint32_t timeDelta;
        ak::uint8_t FPUBuffer;
    };
}