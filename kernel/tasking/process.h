//
// Created by Krisna Pranav on 18/01/22.
//

#pragma once

#include "thread.h"
#include <ak/list.h>
#include <ak/types.h>
#include <libc/ipc.h>
#include <memory/stream.h>

namespace Kernel {
    class symbolDebugger;

    /**
     * processState to printout the process is [New, Active, Inactive, Terminated]
     */
    enum processState {
        New,
        Active,
        InActive,
        Terminated
    };

    #define PROC_USER_HEAP_SIZE 1_MB 

    struct Thread;

    struct Process {
        int id;
        int syscallID;
        bool isUserspace;
        char* args;

        processState state;
        List<Thread*> Threads;
        ak::uint32_t pageDirPhys;

        struct executable {
            ak::uint32_t memBase;
            ak::uint32_t memSize;
        } executable_t;
            
        struct heap {
            ak::uint32_t heapStart;
            ak::uint32_t heapEnd;
        } heap_t;


        List<IPC::IPCMessage> ipcMessages;

        Stream* stdInput;
        Stream* stdOutput;

        char fileName[32];

        symbolDebugger* symDebugger = 0;
    };

    class processHelper {
      public:
        static List<Process*> Processes;
        static Process* create(char* fileName, char* arguments = 0, bool isKernel = false);
        static Process* createKernelProcess();
        static void removeProcess(Process* proc);
        static void updateHeap(Process* proc, ak::uint32_t newEndAddr);
        static Process* processById(int id);

      private:
        processHelper();
    };
}