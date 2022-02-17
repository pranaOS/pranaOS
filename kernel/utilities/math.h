/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <stdint.h>

/**
 * @brief align down
 * 
 */
#define ALIGN_DOWN(base, size) ((base) & -((__typeof__(base))(size)))

/**
 * @brief rand
 * 
 * @return uint32_t 
 */
uint32_t rand();

/**
 * @brief strand
 * 
 * @param seed 
 * @return uint32_t 
 */
uint32_t strand(uint32_t seed);