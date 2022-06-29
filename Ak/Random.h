/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

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