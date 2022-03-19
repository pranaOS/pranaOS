/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <ak/limits.h>
#include <memory/vmm.h>
#include <libkernel/math.h>
#include <libkernel/string.h>
#include "psf.h"

#define MAGIC 0x864ab572

static uint16_t *unicode;
static char *psf_start;

/**
 * @brief putchar
 * @returns nothing
 */
void putchar(uint32_t c, uint32_t cx, uint32_t cy, uint32_t fg, uint32_t bg, char *fb, uint32_t scanline) {
}

/**
 * @brief psf init 
 * 
 * @param buff 
 * @param size 
 */
void psf_init(char *buff, size_t size) {
    psf_start = buff;
    uint16_t glyph = 0;
    char *psf_end = psf_start + size;
}
