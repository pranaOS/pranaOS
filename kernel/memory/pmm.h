/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <multiboot/multiboot.h>
#include "info.h"

/**
 * @brief pmm frames per byte
 * 
 */
#define PMM_FRAMES_PER_BYTE 8

/**
 * @brief pmm frames size
 * 
 */
#define PMM_FRAME_SIZE 4096