#pragma once

#include "types.h"

namespace LibC {
    class converter {
    public:
        static char* intToString(int i);

        static char* intToHexString(pranaOSTypes::uint8_t w);
        static char* intToHexString(pranaOSTypes::uint16_t w);
        static char* intToHexString(pranaOSTypes::uint32_t w);

        static int stringToInt(char* string);
    };
}
