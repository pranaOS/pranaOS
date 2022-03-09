/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

/**
 * @brief sgttyb 
 * 
 */
struct sgttyb {
    char ispeed;
    char ospeed;
    char erase;
    char kill;
    short flags;
};

/**
 * @brief tchars
 * 
 */
struct tchars {
	char t_intrc;
	char t_quitc;
	char t_startc;
	char t_stopc;
	char t_eofc;
	char t_brkc;
};

/**
 * @brief ltchars
 * 
 */
struct ltchars {
	char lt_suspc;
	char lt_dsuspc;
	char lt_rprntc;
	char lt_flushc;
	char lt_werasc;
	char lt_lnextc;
};