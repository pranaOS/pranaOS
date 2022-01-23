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

        static void setRegionFree(ak::uint32_t base, ak::uint32_t size);
        static void setRegionUsed(ak::uint32_t base, ak::uint32_t size);
        static parseMemoryMap(const multiboot_info_t* mbi);

        static void* allocateBlock();
        static void freeBlock(void* ptr);

        static allocateBlocks(ak::uint32_t size);
        static freeBlocks(void* ptr, ak::uint32_t size);

        static ak::uint32_t amountOfMemory();
        static ak::uint32_t usedBlocks();
        static ak::uint32_t freeBlocks();
        static ak::uint32_t totalBlocks();
        static ak::uint32_t getBitmapSize();
      private:
        physicalMemoryManager();
    };

    ak::uint32_t pageRoundUp(ak::uint32_t address);
    ak::uint32_t pageRoundDown(ak::uint32_t address);
}
