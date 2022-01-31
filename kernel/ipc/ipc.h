//
// Created by KrisnaPranav on 23/01/22.
//

#pragma once

#include <kernel/cpu/register.h>
#include <kernel/tasking/process.h>

namespace Kernel {
    struct ipcReceiveDescriptor {
        Process* receivingProcess;
        Thread* receivingThread;
        int receiveFromPID;
        int receiveType;
    };

    class ipcManager {
      public:
        static void initialize();
        static void handleSend(Kernel::CPUState* state, Process* proc);
        static void handleReceive(Kernel::CPUState* state, Process* proc);
    };
}