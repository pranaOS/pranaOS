/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <stdarg.h>
#include <stdint.h>
#include <multiboot/multiboot.h>

/**
 * @brief framebuffer
 * 
 */
struct framebuffer {
    /**
     * @brief addr, pitch, width, height 
     * 
     */
    uint32_t addr;
    uint32_t pitch;
    uint32_t width;
    uint32_t height;

    /**
     * @brief bpp
     * 
     */
    uint8_t bpp;
};
