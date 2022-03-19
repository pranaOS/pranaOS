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
 * @brief psf init 
 * 
 * @param buff 
 * @param size 
 */
void psf_init(char *buff, size_t size);