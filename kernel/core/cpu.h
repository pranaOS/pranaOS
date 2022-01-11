#pragma once

#include <ak/types.h>

namespace Core {
        #define EDX_SSE2 (1 << 26) 
        #define EDX_FXSR (1 << 24) 

        class CPU {
        public:
            static void PrintVendor();
            static void EnableFeatures();
        };        
}