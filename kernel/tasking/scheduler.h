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

      private:
        bool switchForced = false;
    };
}
