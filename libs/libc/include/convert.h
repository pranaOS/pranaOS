#pragma once

#include <types.h>

namespace pranaOSConvert {
    class Convert {
    public:
        static char* intToString(int n);
        static char* intToHexString(uint8_t w);
        static char* intToHexString(uint16_t w);
        static char* intToHexString(uint32_t w);
    };
}