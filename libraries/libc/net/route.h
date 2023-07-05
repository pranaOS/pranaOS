/**
 * @file route.h
 * @author Krisna Pranav
 * @brief route
 * @version 1.0
 * @date 2023-07-05
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <sys/socket.h>

struct rtentry {
    struct sockaddr rt_gateway;
    struct sockaddr rt_genmask;
    unsigned short int rt_flags;
    char* rt_dev;
};

#define RTF_UP 0x1
#define RTF_GATEWAY 0x2
