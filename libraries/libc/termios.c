/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <sys/ioctl.h>
#include <termios.h>
#include <assert.h>

/**
 * @brief setattr
 * 
 * @param fd 
 * @param opts 
 * @param tp 
 * @return int 
 */
int setattr(int fd, int opts, const struct termios *tp) {
	int cmd;
	switch (opts) {
	case TCSANOW:
		cmd = TCSETS;
		break;
	case TCSADRAIN:
		cmd = TCSETSW;
		break;
	case TCSAFLUSH:
        cmd = TCSETSF;
		break;
	default:
		assert_not_reached();
	}

	return ioctl(fd, cmd, tp);
}

/**
 * @brief cflow 
 * 
 * @param fd 
 * @param action 
 * @return int 
 */
int cflow(int fd, int action) {
    return ioctl(fd, TCXONC, action);
}

/**
 * @brief getispeed
 * 
 * @param termios_p 
 * @return speed_t 
 */
speed_t getispeed(const struct termios *termios_p) {
	assert_not_reached();
	__builtin_unreachable();
}

/**
 * @brief getospeed
 * 
 * @param termios_p 
 * @return speed_t 
 */
speed_t getospeed(const struct termios *termios_p) {
	assert_not_reached();
	__builtin_unreachable();
}

/**
 * @brief setispeed
 * 
 * @param termios_p 
 * @param speed 
 * @return int 
 */
int setispeed(struct termios *termios_p, speed_t speed) {
	assert_not_reached();
	__builtin_unreachable();
}

/**
 * @brief setospeed
 * 
 * @param termios_p 
 * @param speed 
 * @return int 
 */
int setospeed(struct termios *termios_p, speed_t speed) {
	assert_not_reached();
	__builtin_unreachable();
}