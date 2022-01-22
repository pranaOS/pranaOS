//
// Created by KrisnaPranav on 22/01/22.
//

#pragma once

#include <ak/types.h>
#include <ak/convert.h>
#include <ak/memoperator.h>
#include <system/console.h>
#include <multiboot/multiboot.h>

namespace Kernel {
    #define BLOCK_SIZE 4_KB
    #define BLOCKS_PER_BYTE 8

    typedef struct multibootMemoryMap {
        unsigned int size;
        unsigned long baseAddrLow;
        unsigned long baseAddrHigh;
        unsigned long lengthLow;
        unsigned long lengthHigh;
        unsigned int type;
    } __attribute__((packed)) grub_multiboot_memory_t;

    class physicalMemoryManager {
      public:
        static void initialize(ak::uint32_t size, ak::uint32_t bitmap);
    };
}
