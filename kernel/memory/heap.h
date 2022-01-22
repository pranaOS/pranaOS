#pragma once

#include <cpu/vm.h>
#include <tasking/lock.h>

namespace Kernel {
    #define KERNEL_HEAP_START (KERNEL_VIRT_ADDR + 4_MB)
    #define KERNEL_HEAP_SIZE 16_MB
    #define MINIMAL_SPLIT_SIZE 4 
    #define MEMORY_HEADER_MAGIC 1234567890

    #ifndef align_up
    #define align_up(num, align) \
        (((num) + ((align) - 1)) & ~((align) - 1))
    #endif

    struct memoryHeader {
        ak::uint32_t magic;
        ak::uint32_t allocated;
        ak::uint32_t size;

        memoryHeader* next;
        memoryHeader* prev;
    } __attribute__((packed));

    class kernelHeap {
    };
}