#pragma once

#include <systeminfo.h>
#include <syscall.h>
#include <types.h>
#include <list.h>
#include <shared.h>

namespace pranaOSProc {
    #define DECLARE_LOCK(name) volatile int name ## Locked

    #define LOCK(name) \
	    while (name ## Locked == 1) asm("pause"); \
	    __sync_synchronize();

    #define UNLOCK(name) \
	    __sync_synchronize(); \
	    name ## Locked = 0;

    class Process {
    public:
        static int ID;
        static sharedSystemInfo* systemInfo;

        static int run(const char* path, bool black = false);
        
        static bool createSharedMemory(int proc2ID, uint32_t virtStart, uint32_t len); 
        static bool createSharedMemory(int proc2ID, uint32_t virtStart1, uint32_t virtStart2, uint32_t len);

        static bool deleteSharedMemory(int proc2ID, uint32_t virtStart, uint32_t len);

    private:
        static int numThreads;
    };
}