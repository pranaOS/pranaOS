#pragma once

#include "types.h"
#include <core/port.h>

namespace pranaOS {
    namespace system {
        enum COMPort {
            COM1 = 0x3F8,
            COM2 = 0x2F8,
            COM3 = 0x3E8,
            COM4 = 0x2E8,
        };

        class Serialport {
        public:
            static int serialreceiveready();
            static serialsendready();

            static bool initialized;
            static void init(COMPort port);

            static char read();
        };
    }
}