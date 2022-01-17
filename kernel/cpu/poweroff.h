#pragma once

#include "port.h"

namespace Kernel {
    class powerOff {
    public:
        static void initialize();
        static void poweroff();
    };
}
