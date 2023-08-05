/**
 * @file termios.h
 * @author Krisna Pranav
 * @brief termios
 * @version 1.0
 * @date 2023-08-05
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

typedef uint32_t tcflag_t;
typedef uint8_t cc_t;
typedef uint32_t speed_t;

struct termios 
{
    tcflag_t c_iflag;
    tcflag_t c_oflag;
    tcflag_t c_cflag;
    tcflag_t c_lflag;
    cc_t c_cc[NCCS];
    speed_t c_ispeed;
}; // struct termios

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
speed_t cfgetisspeed(const struct termios*);

/**
 * @return speed_t 
 */
speed_t cfgetospeed(const struct termios*);


__END_DECLS