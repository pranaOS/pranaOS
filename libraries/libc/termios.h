/**
 * @file termios.h
 * @author Krisna Pranav
 * @brief termios
 * @version 6.0
 * @date 2023-08-05
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/api/posix/termios.h>

__BEGIN_DECLS

/**
 * @param fd 
 * @return int 
 */
int tcdrain(int fd);

/**
 * @param fd 
 * @return int 
 */
int tcgetattr(int fd, struct termios*);

/**
 * @param fd 
 * @param optional_actions 
 * @return int 
 */
int tcsetattr(int fd, int optional_actions, const struct termios*);

/**
 * @param fd 
 * @param duration 
 * @return int 
 */
int tcsendbreak(int fd, int duration);

/**
 * @param fd 
 * @param action 
 * @return int 
 */
int tcflow(int fd, int action);

/**
 * @param fd 
 * @param queue_selector 
 * @return int 
 */
int tcflush(int fd, int queue_selector);

/**
 * @return speed_t 
 */
speed_t cfgetispeed(const struct termios*);

/**
 * @return speed_t 
 */
speed_t cfgetospeed(const struct termios*);

/**
 * @return int 
 */
int cfsetispeed(struct termios*, speed_t);

/**
 * @return int 
 */
int cfsetospeed(struct termios*, speed_t);

void cfmakeraw(struct termios*);

__END_DECLS