/**
 * @file ifaddrs.h
 * @author Krisna Pranav
 * @brief IF address
 * @version 6.0
 * @date 2024-09-07
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct ifaddrs 
{
    struct ifaddrs* ifa_next;
    char* ifa_name;
    unsigned int ifa_flags;
    struct sockaddr* ifa_addr;
    struct sockaddr* ifa_netmask;
    union 
    {
        struct sockaddr* ifu_broadaddr;
        struct sockaddr* ifu_dstaddr;
    } ifa_ifu;
    
#define ifa_broadaddr ifa_ifu.ifu_broadaddr
#define ifa_dstaddr ifa_ifu.ifu_dstaddr
    void* ifa_data;
};

#ifdef __cplusplus
}
#endif