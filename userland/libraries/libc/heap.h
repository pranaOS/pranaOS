#pragma once

#include <types.h>

namespace LibC {
    uint32_t pageRoundUp(uint32_t address);
    uint32_t pageRoundDown(uint32_t address);

    #define HEAP_SIZE 10_MB

    struct memoryHeader {
        memoryHeader* next;
        memoryHeader* prev;
        bool allocated;
        uint32_t size;
        size_t size;
    } __attribute__((packed));

    class userHeap {
    public:
        static void initialize();
        static void printMemoryLayout();

        static void* malloc(uint32_t size);
        static void free(void* ptr);

        static void* alignedMalloc(uint32_t size, uint32_t align);
        static void alignedFree(void* ptr);

    private:
        static uint32_t startAddress;
        static uint32_t endAddress;
        static uint32_t maxAddress;

        static memoryHeader* firstHeader;
    };
}