#pragma once

#include "types.h"

namespace pranaOS {
    namespace ak {
        class Random {
        public:
            static int next(uint32_t max = 32767);
            static int next(uint32_t min, uint32_t max);
            static void setseed(uint32_t setseed);
        };
    }
}