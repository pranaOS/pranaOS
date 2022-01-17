#pragma once

#include <ak/types.h>
#include <core/port.h>

namespace Kernel {
    enum COMPort
    {
        COM1 = 0x3F8,
        COM2 = 0x2F8,
        COM3 = 0x3E8,
        COM4 = 0x2E8
    };

    class serialPort {
    public:
        static int serialReceiveReady();
        static int serialSendReady();

        static bool initialized;
        static void init(COMPort port);

        static char read();
        static void write(char a);
        static void writeStr(char* str);

    private:
        static COMPort portaddress;
    };
}