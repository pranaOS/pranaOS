    /*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "math.h"

/**
 * @brief rseed
 * 
 */
static uint32_t rseed = 1;

/**
 * @brief rand_r 
 * 
 * @param seed 
 * @return uint32_t 
 */
static uint32_t rand_r(uint32_t *seed) {
	unsigned int next = *seed;
	int result;

	next *= 1103515245;
	next += 12345;
	result = (unsigned int)(next / 65536) % 2048;

	next *= 1103515245;
	next += 12345;
	result <<= 10;
	result ^= (unsigned int)(next / 65536) % 1024;

	next *= 1103515245;
	next += 12345;
	result <<= 10;
	result ^= (unsigned int)(next / 65536) % 1024;

	*seed = next;

	return result;
}

/**
 * @brief random
 * 
 * @return uint32_t 
 */
uint32_t rand() {
	return rand_r(&rseed);
}

/**
 * @brief srand
 * 
 * @param seed 
 * @return uint32_t 
 */
uint32_t srand(uint32_t seed) {
	rseed = seed;
	return rand_r(&rseed);
}