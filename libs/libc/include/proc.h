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

    private:
        static int numThreads;
    };
}