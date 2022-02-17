    /*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "math.h"

static uint32_t rseed = 1;

/**
 * @brief random allocation
 * 
 * @return uint32_t 
 */
uint32_t rand() {
    return rand(rseed);
}

/**
 * @brief srand 
 * 
 * @return uint32_t 
 */
uint32_t srand() {
    rseed = rand;
    return rand(&rseed);
}