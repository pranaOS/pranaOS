/**
 * @file ioctl.h
 * @author Krisna Pranav
 * @brief IOCTL
 * @version 0.1
 * @date 2023-05-25
 * 
 * @copyright Copyright (c) 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#define TIOCSCTTY 	1
#define TIOCGPGRP	2
#define TIOCSPGRP	3
#define TCGETS		4
#define TCSETS		5
#define TCSETSW		6
#define TCSETSF		7
#define TCFLSH		8
#define TIOCSWINSZ	9
#define TIOCGWINSZ	10
#define TIOCNOTTY	11
#define TIOSGFX		12
#define TIOSNOGFX	13