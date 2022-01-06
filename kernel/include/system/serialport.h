#pragma once

#include <ak/types.h>
#include <core/port.h>

namespace pranaOS {
    namespace system {

        enum COMPort
        {
            COM1 = 0x3F8,
            COM2 = 0x2F8,
            COM3 = 0x3E8,
            COM4 = 0x2E8
        };

        class Serialport {
        public:
            static int serialreceiveready();
            static int serialsendready();

            static bool initialized;
            static void init(COMPort port);

            static char read();
            static void write(char a);
            static void writestr(char* str);

        private:
            static COMPort portaddress;
        };
    }
}