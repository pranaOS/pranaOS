/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */


#include "random.h"

using namespace ak;

uint32_t Random::next;

int Random::next(uint32_t max) {
    next = next * 1103515245 + 12345;

    return (unsigned int)(next / 65536) % (max+1);
}

int Random::next(uint32_t min, uint32_t max) {
    return next(max-min) + min;
}

void Random::setSeed(uint32_t seed) {
    next = seed;
}