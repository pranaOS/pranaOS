/**
 * @file termios.cpp
 * @author Krisna Pranav
 * @brief termios
 * @version 0.1
 * @date 2023-08-05
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <Kernel/API/Syscall.h>
#include <assert.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <termios.h>

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
        case TCSAFLUSH:
            return ioctl(fd, TCSETSF, t);
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
     * @return int 
     */
    static int baud_rate_from_speed(speed_t speed)
    {
        int rate = -EINVAL;
        switch (speed) {
        case B0:
            rate = 0;
            break;
        case B50:
            rate = 50;
            break;
        case B75:
            rate = 75;
            break;
        case B110:
            rate = 110;
            break;
        case B134:
            rate = 134;
            break;
        case B150:
            rate = 150;
            break;
        case B200:
            rate = 200;
            break;
        case B300:
            rate = 300;
            break;
        case B600:
            rate = 600;
            break;
        case B1200:
            rate = 1200;
            break;
        case B1800:
            rate = 1800;
            break;
        case B2400:
            rate = 2400;
            break;
        case B4800:
            rate = 4800;
            break;
        case B9600:
            rate = 9600;
            break;
        case B19200:
            rate = 19200;
            break;
        case B38400:
            rate = 38400;
            break;
        }

        return rate;
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

    /**
     * @param tp 
     * @param speed 
     * @return int 
     */
    int cfsetospeed(struct termios* tp, speed_t speed)
    {
        auto ospeed = baud_rate_from_speed(speed);

        if (ospeed > 0) {
            tp->c_ispeed = ospeed;
        }

        __RETURN_WITH_ERRNO(ospeed, 0, -1);
    }
} // extern
