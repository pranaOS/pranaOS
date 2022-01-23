//
// Created by KrisnaPranav on 23/01/22.
//

#pragma once

#include <kernel/interrupthandler.h>

namespace Kernel {
    class syscallHandler : public interruptHandler {
      public:
        syscallHandler();

        ak::uint32_t handleInterrupt(ak::uint32_t esp);
    };
}