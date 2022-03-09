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

/**
 * @brief winsize
 * 
 */

struct winsize {
    unsigned short row;
    unsigned short col;
    unsigned short xpixel;
    unsigned short ypixel;
};

/**
 * @brief cc_t
 * 
 */
typedef unsigned char cc_t;

/**
 * @brief speed_t
 * 
 */
typedef unsigned int speed_t;

/**
 * @brief cflags_t
 * 
 */
typedef unsigned int tcflag_t;

#define NCCS 19

struct termios {
    tcflag_t cflag;
    tcflag_t c_oflag;
	tcflag_t c_cflag;
	tcflag_t c_lflag;
	cc_t c_line;
	cc_t c_ccc[NCCS];
};

#define VINTR 0
#define VQUIT 1
#define VERASE 2
#define VKILL 3
#define VEOF 4
#define VTIME 5
#define VMIN 6
#define VSWTC 7
#define VSTART 8
#define VSTOP 9
#define VSUSP 10
#define VEOL 11
#define VREPRINT 12
#define VDISCARD 13
#define VWERASE 14
#define VLNEXT 15
#define VEOL2 16