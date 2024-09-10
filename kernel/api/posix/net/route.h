/**
 * @file route.h
 * @author Krisna Pranav
 * @brief route
 * @version 6.0
 * @date 2024-09-10
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/api/posix/sys/socket.h>
#include <kernel/api/posix/sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

struct rtentry 
{
    struct sockaddr rt_gateway; 
    struct sockaddr rt_genmask; 
    unsigned short int rt_flags;
    char* rt_dev;
}; // struct rtentry

#define RTF_UP 0x1      
#define RTF_GATEWAY 0x2 

#ifdef __cplusplus
}
#endif
