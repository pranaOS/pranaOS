#pragma once

#include <ak/types.h>
#include <kernel/memory/heap.h>
#include "physicalmemory.h"

namespace Kernel {
    extern "C" void* bootPageDirectory;

    enum PAGEENTRYESIZE {
        FOUR_KB = 0,
        FOUR_MB = 1
    };

    #define KERNEL_VIRT_ADDR 3_GB
    #define USER_STACK_SIZE 32_KB
    #define USER_STAcK_TOP (KERNEL_VIRT_ADDR)
    #define USER_STACK (USER_STACK_TOP - USER_STACK_SIZE)

    #define PAGE_SIZE 4_KB
    #define KERNEL_PTNUM 768 
    #define PAGE_TABLE_ADDRESS 0xFFC00000
    #define PAGE_DIRECTORY_ADDRESS 0xFFFFF000
}