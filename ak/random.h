/*
 * Copyright (c) 2021-2022, Krisna Pranav
 *
 * SPDX-License-Identifier: GPL-3.0 License
*/


#pragma once

#include "types.h"

namespace pranaOS {
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
}