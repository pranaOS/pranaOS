/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <sys/ioctl.h>
#include <termios.h>

int getattr(int fd, struct termios *term);