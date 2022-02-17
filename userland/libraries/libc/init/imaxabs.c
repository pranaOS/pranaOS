/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <init.h>

/**
 * @brief imaxabs
 * 
 * @param myval 
 * @return intmax_t 
 */
intmax_t imaxabs(intmax_t myval) {
	return myval < 0 ? -myval : myval;
}