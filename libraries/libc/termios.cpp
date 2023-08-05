/**
 * @file termios.cpp
 * @author Krisna Pranav
 * @brief termios
 * @version 0.1
 * @date 2023-08-05
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <assert.h>
#include <errno.h>
#include <sys/termios.h>
#include <termios.h>
#include <kernel/api/syscall.h>
#include <sys/ioctl.h>

extern "C"
{
    /**
     * @param fd 
     * @param t 
     * @return int 
     */
    int tcgetattr(int fd, struct termios* t)
    {
        return ioctl(fd, TCGETS, t);
    }
    
    /**
     * @param fd 
     * @param optional_actions 
     * @param t 
     * @return int 
     */
    int tcsetattr(int fd, int optional_actions, const struct termios* t)
    {
        switch (optional_actions) {
        case TCSANOW:
            return ioctl(fd, TCSETS, t);
        case TCSADRAIN:
            return ioctl(fd, TCSETSW, t);
        }

        errno = EINVAL;

        return -1;
    }

    /**
     * @param fd 
     * @param action 
     * @return int 
     */
    int tcflow(int fd, int action)
    {
        (void)fd;
        (void)action;

        ASSERT_NOT_REACHED();
    }   

    /**
     * @param fd 
     * @param queue_selector 
     * @return int 
     */
    int tcflush(int fd, int queue_selector)
    {
        return ioctl(fd, TCFLSH, queue_selector);
    }

    /**
     * @param tp 
     * @return speed_t 
     */
    speed_t cfgetispeed(const struct termios* tp)
    {
        return tp->c_ispeed;
    }

    /**
     * @param tp 
     * @return speed_t 
     */
    speed_t cfgetospeed(const struct termios* tp)
    {
        return tp->c_ospeed;
    }

    /**
     * @param speed 
     */
    static baud_rate_from_speed(speed_t speed)
    {

    }

    /**
     * @param tp 
     * @param speed 
     * @return int 
     */
    int cfsetispeed(struct termios* tp, speed_t speed)
    {
        auto ispeed = baud_rate_from_speed(speed);

        if (ispeed > 0) {
            tp->c_ispeed = ispeed;
        }

        __RETURN_WITH_ERRNO(ispeed, 0, -1);
    }

} // extern