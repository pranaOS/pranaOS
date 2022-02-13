//
// Created by KrisnaPranav on 13/02/22.
//

#pragma once

#include <tasking/process.h>
#include <stdint.h>

namespace Kernel {
    class sharedMemory {
      public:
        static bool CreateSharedRegion(Process* proc1, Process* proc2, uint32_t virtStart, uint32_t len);

    };
}