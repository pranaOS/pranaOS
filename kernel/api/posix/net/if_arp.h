/**
 * @file if_arp.h
 * @author Krisna Pranav
 * @brief if arp
 * @version 6.0
 * @date 2024-09-10
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <kernel/api/posix/sys/socket.h>
#include <kernel/api/posix/sys/types.h>

struct arpreq 
{
    struct sockaddr arp_pa;      
    struct sockaddr arp_ha;      
    struct sockaddr arp_netmask; 
    int arp_flags;               
    char arp_dev[16];
}; // struct arpreq

#define ARPHRD_ETHER 1
#define ARPHRD_IEEE802 6
#define ARPHRD_SLIP 256
#define ARPHRD_PPP 512
#define ARPHRD_LOOPBACK 772
#define ARPHRD_FDDI 774
#define ARPHRD_IEEE802_TR 800

#ifdef __cplusplus
}
#endif
