#pragma once

#include "port.h"

namespace Kernel {
    class reboot {
    public:
        static void initialize();
        static void reboot();
    };
}
