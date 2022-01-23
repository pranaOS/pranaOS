//
// Created by KrisnaPranav on 23/01/22.
//

#pragma once

#include <kernel/cpu/register.h>

namespace Kernel {
    class pranaOSSyscalls {
      public:
        static Kernel::CPUState* handleSyscalls(Kernel::CPUState* state);
    };
}