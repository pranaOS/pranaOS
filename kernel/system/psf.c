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
 * 
 * @param c 
 * @param cx 
 * @param cy 
 * @param fg 
 * @param bg 
 * @param fb 
 * @param scanline 
 */
void putchar(uint32_t c, uint32_t cx, uint32_t cy, uint32_t fg, uint32_t bg, char *fb, uint32_t scanline) {
    struct psf_t *font = (struct psf_t *)psf_start;
    int bytesperline = div_ceil(font->width, 8);
    if (unicode != NULL) {
        c = unicode[c];
    }

    unsigned char *glyph = (unsigned char *)psf_start + font->headersize + (c > 0 && c < font->numglyph ? c : 0) * font->bytesperglyph;

    int offs = cy * scanline + cx * 4;
    uint32_t x, y, line, mask;
    for (y = 0; y < font->height; y++) {
		line = offs;
		mask = 1 << (font->width - 1);
		for (x = 0; x < font->width; x++) {
			*((uint32_t *)(fb + line)) = ((int)*glyph) & (mask) ? fg : bg;
			mask >>= 1;
			line += 4;
		}
		glyph += bytesperline;
		offs += scanline;
	}
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
