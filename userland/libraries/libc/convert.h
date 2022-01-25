#pragma once

#include "types.h"

namespace LibC {
    class converter {
    public:
        static char* intToString(int i);

        static char* intToHexString(LibC::uint8_t w);
        static char* intToHexString(LibC::uint16_t w);
        static char* intToHexString(LibC::uint32_t w);

        static int stringToInt(char* string);
    };
}
