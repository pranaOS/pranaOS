#pragma once

#include <ak/types.h>

namespace Kernel {
        #define EDX_SSE2 (1 << 26) 
        #define EDX_FXSR (1 << 24) 

        class Cpu {
        public:
            static void printVendor();
            static void enableFeatures();
        };        
}
