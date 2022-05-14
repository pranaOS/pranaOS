/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <sys/ioctl.h>
#include <termio.h>

int getattr(int fd, struct termios *term) {
  	return ioctl(fd, TCGETS, (unsigned long)term);
}
