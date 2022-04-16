//
// Created by KrisnaPranav on 23/01/22.
//

#pragma once

#include <ak/types.h>
#include <kernel/interrupthandler.h>
#include "thread.h"

namespace Kernel {
    #define SCHEDULER 30

    class scheduler : public interruptHandler {
      public:
        scheduler();
        bool enabled = true;

        uint32_t handleInterrupts(uint32_t esp);

        void addThread(Thread* thread, bool forceSwitch = false);
        void forceSwitch();
        void initialThreadUserJump(Thread* thread);
        void block(Thread* thread, blockedState reason = blockedState::Unkown);
        void unblock(Thread* thread, bool forceSwitch = false);

        Thread* currentThread();
        Process* currentProcess();

      private:
        uint32_t frequency = 0;
        uint32_t tickCount = 0;

        list<Thread*> threadLists;
        Thread* currentThread = 0;

        Thread* getNextThread();
        void processSleepingThreads();
        bool switchForced = false;
    };
}
