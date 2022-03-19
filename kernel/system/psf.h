/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

#define FONT_MAGIC 0x864ab572
#define UNICODE_TABLE 0x01

struct psf_t {
	uint32_t magic;
	uint32_t version;
	uint32_t headersize;
	uint32_t flags;
	uint32_t numglyph;
	uint32_t bytesperglyph;
	uint32_t height;
	uint32_t width;
};

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
void putchar(uint32_t c, uint32_t cx, uint32_t cy, uint32_t fg, uint32_t bg, char *fb, uint32_t scanline);

/**
 * @brief puts
 * 
 * @param s 
 * @param cx 
 * @param cy 
 * @param fg 
 * @param bg 
 * @param fb 
 * @param scanline 
 */
void puts(const char *s, uint32_t cx, uint32_t cy, uint32_t fg, uint32_t bg, char *fb, uint32_t scanline);

/**
 * @brief psf init 
 * 
 * @param buff 
 * @param size 
 */
void psf_init(char *buff, size_t size);
