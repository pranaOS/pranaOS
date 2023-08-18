/**
 * @file ioctl_numbers.h
 * @author Krisna Pranav
 * @brief ioctl numbers
 * @version 6.0
 * @date 2023-07-07
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <sys/cdefs.h>

__BEGIN_DECLS

/// @brief winsize
struct winsize {
    unsigned short ws_row;
    unsigned short ws_col;
    unsigned short ws_xpixel;
    unsigned short ws_ypixel;
};

/// @brief fbresolution
struct FBResolution {
    unsigned pitch;
    unsigned width;
    unsigned height;
};

__END_DECLS

/// @brief ioctl number codes
enum IOCtlNumber {
    TIOCGPGRP,
    TIOCSPGRP,
    TCGETS,
    TCSETS,
    TCSETSW,
    TCSETSF,
    TCFLSH,
    TIOCGWINSZ,
    TIOCSCTTY,
    TIOCNOTTY,
    TIOCSWINSZ,
    FB_IOCTL_GET_SIZE_IN_BYTES,
    FB_IOCTL_GET_RESOLUTION,
    FB_IOCTL_SET_RESOLUTION,
    FB_IOCTL_GET_BUFFER,
    FB_IOCTL_SET_BUFFER,
    SIOCSIFADDR,
    SIOCGIFADDR,
    SIOCGIFHWADDR,
    SIOCSIFNETMASK,
    SIOCADDRT,
    SIOCDELRT
};

#define TIOCGPGRP TIOCGPGRP
#define TIOCSPGRP TIOCSPGRP
#define TCGETS TCGETS
#define TCSETS TCSETS
#define TCSETSW TCSETSW
#define TCSETSF TCSETSF
#define TCFLSH TCFLSH
#define TIOCGWINSZ TIOCGWINSZ
#define TIOCSCTTY TIOCSCTTY
#define TIOCNOTTY TIOCNOTTY
#define TIOCSWINSZ TIOCSWINSZ
#define FB_IOCTL_GET_SIZE_IN_BYTES FB_IOCTL_GET_SIZE_IN_BYTES
#define FB_IOCTL_GET_RESOLUTION FB_IOCTL_GET_RESOLUTION
#define FB_IOCTL_SET_RESOLUTION FB_IOCTL_SET_RESOLUTION
#define FB_IOCTL_GET_BUFFER FB_IOCTL_GET_BUFFER
#define FB_IOCTL_SET_BUFFER FB_IOCTL_SET_BUFFER
#define SIOCSIFADDR SIOCSIFADDR
#define SIOCGIFADDR SIOCGIFADDR
#define SIOCGIFHWADDR SIOCGIFHWADDR
#define SIOCSIFNETMASK SIOCSIFNETMASK
#define SIOCADDRT SIOCADDRT
#define SIOCDELRT SIOCDELRT
