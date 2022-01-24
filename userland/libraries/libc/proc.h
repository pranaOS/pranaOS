#pragma once

#include <systeminfo.h>
#include <syscall.h>
#include <types.h>
#include "list/list.h"
#include <shared.h>

namespace LibC {

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
 
        static int run(const char* path, bool block = false);

        static bool createSharedMemory(int proc2ID, uint32_t virtStart, uint32_t len);
        static bool createSharedMemory(int proc2ID, uint32_t virtStart1, uint32_t virtStart2, uint32_t len);

        static bool deleteSharedMemory(int proc2ID, uint32_t virtStart, uint32_t len);
        static bool deleteSharedMemory(int proc2ID, uint32_t virtStart1, uint32_t virtStart2, uint32_t len);

        static void createThread(void (*entryPoint)(), bool switchTo = false);

        static void yield();

        static void writeStdOut(char byte);
        static void writeStdOut(char* bytes, int length);

        static char readStdIn();

        static int stdInAvailable();

        static void bindSTDIO(int fromID, int toID);

        static bool active(int pid);

        static void inblock(int procPID, int thread = 0);

        static void setScheduler(bool active);
    
    private:
        static int numThreads;
    };
}