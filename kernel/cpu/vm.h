#pragma once

#include <ak/types.h>
#include <kernel/memory/heap.h>
#include "physicalmemory.h"

namespace Kernel {
    extern "C" void* bootPageDirectory;

    enum PAGE_ENTRY_SIZE {
        FOUR_KB = 0,
        FOUR_MB = 1
    };

    #define KERNEL_VIRT_ADDR 3_GB
    #define USER_STACK_SIZE 32_KB
    #define USER_STACK_TOP (KERNEL_VIRT_ADDR)
    #define USER_STACK (USER_STACK_TOP - USER_STACK_SIZE)

    #define PAGE_SIZE 4_KB
    #define KERNEL_PTNUM 768 
    #define PAGE_TABLE_ADDRESS 0xFFC00000
    #define PAGE_DIRECTORY_ADDRESS 0xFFFFF000
    #define PAGE_OFFSET_BITS 12
    #define PAGEDIR_INDEX(addr) (((uint32_t)addr) >> 22)
    #define PAGETBL_INDEX(addr) ((((uint32_t)addr) >> 12) & 0x3ff)
    #define PAGEFRAME_INDEX(addr) (((uint32_t)addr) & 0xfff)

    struct pageDirectoryEntry {
        uint32_t present        : 1;    
        uint32_t readWrite      : 1;    
        uint32_t isUser         : 1;    
        uint32_t writeThrough   : 1;    
        uint32_t canCache       : 1;    
        uint32_t accessed       : 1;    
        uint32_t reserved       : 1;    
        uint32_t pageSize       : 1;    
        uint32_t ignored        : 1;
        uint32_t unused         : 3;    
        uint32_t frame          : 20;   
    } __attribute__((packed));

    struct pageTableEntry {
        uint32_t present        : 1;    
        uint32_t readWrite      : 1;    
        uint32_t isUser         : 1;    
        uint32_t writeThrough   : 1;    
        uint32_t canCache       : 1;    
        uint32_t accessed       : 1;    
        uint32_t dirty          : 1;    
        uint32_t reserved       : 1;
        uint32_t global         : 1;    
        uint32_t unused         : 3;    
        uint32_t frame          : 20;   
    } __attribute__((packed));

    struct pageTable {
        pageTableEntry entries[1024];
    } __attribute__((packed));

    struct pageDirectory {
        pageDirectoryEntry entries[1024];
    } __attribute__((packed));

    static inline void invlpg(void* addr) {
        asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
    }

    class virtualMemoryManager {
    public:      
        static void reloadCR3();  
        static void initialize();
        static void allocatePage(pageTableEntry* page, bool kernel, bool writeable);
        static void freePage(pageTableEntry* page);

        static pageTableEntry* getPageForAddress(uint32_t virtualAddress, bool shouldCreate, bool readWrite = true, bool userPages = false);    
        static void* getPageTableAddress(uint16_t pageTableNumber);
            
        static void* virtualToPhysical(void* virtAddress);
            
        static void mapVirtualToPhysical(void* physAddress, void* virtAddress, bool kernel = true, bool writeable = true);
        static void mapVirtualToPhysical(void* physAddress, void* virtAddress, uint32_t size, bool kernel = true, bool writeable = true);
            
        static void switchPageDirectory(uint32_t physAddr);
        static uint32_t getPageDirectoryAddress();
    private:
        virtualMemoryManager();
    };
}