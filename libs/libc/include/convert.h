#pragma once

#include <types.h>

namespace pranaOSConvert {
    class Convert {
    public:
        static char* IntToString(int i);

        static char* IntToHexString(uint8_t w);
        static char* IntToHexString(uint16_t w);
        static char* IntToHexString(uint32_t w);

        static int StringToInt(char* string);
    };
}