#pragma once

#include "types.h"

namespace ak {
    
    class Random {
    public:
        static int next(uint32_t max = 32767);
        static int next(uint32_t min, uint32_t max);             
        static void setSeed(uint32_t seed);
    private:
        static uint32_t next;
    };
    
}