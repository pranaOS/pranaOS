#pragma once

#include "types.h"
#include "convert.h"

namespace pranaOS {
    namespace ak {
        class Print {
        public:
            static void printfhex(uint8_t key);
            static void printfhex16(uint16_t key);
            static void printfhex32(uint32_t key);
            static void printbits(uint8_t key);
            static void printbits(uint16_t key);  
        };
    }
}