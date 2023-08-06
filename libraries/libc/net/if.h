/**
 * @file if.h
 * @author Krisna Pranav
 * @brief if
 * @version 1.0
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <sys/cdefs.h>
#include <sys/socket.h>

__BEGIN_DECLS


struct ifreq 
{
    #define IFNAMESIZ 16
    char ifr_name[IFNAMESIZ];

    union
    {
        struct sockaddr ifru_addr;
        struct sockaddr ifru_dstaddr;
        struct sockaddr ifru_brodaddr;
        struct sockaddr ifru_hwaddr;
        short ifru_flags;
        int ifru_flags;
        int64_t ifru_vnetid;
        void* ifru_data;
    } ifr_ifru;
}

__END_DECLS