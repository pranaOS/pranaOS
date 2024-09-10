/**
 * @file if.h
 * @author Krisna Pranav
 * @brief if
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

enum 
{
    IFF_UP = 1 << 0,
    IFF_BROADCAST = 1 << 1,
    IFF_DEBUG = 1 << 2,
    IFF_LOOPBACK = 1 << 3,
    IFF_POINTOPOINT = 1 << 4,
    IFF_RUNNING = 1 << 5,
    IFF_NOARP = 1 << 6,
    IFF_PROMISC = 1 << 7,
    IFF_ALLMULTI = 1 << 8,
    IFF_MULTICAST = 1 << 9,
};

#define IFF_UP IFF_UP
#define IFF_BROADCAST IFF_BROADCAST
#define IFF_DEBUG IFF_DEBUG
#define IFF_LOOPBACK IFF_LOOPBACK
#define IFF_POINTOPOINT IFF_POINTOPOINT
#define IFF_RUNNING IFF_RUNNING
#define IFF_NOARP IFF_NOARP
#define IFF_PROMISC IFF_PROMISC
#define IFF_ALLMULTI IFF_ALLMULTI
#define IFF_MULTICAST IFF_MULTICAST

struct ifconf
{
    int ifc_len;
    union 
    {
        void* ifc_buf;
        struct ifreq* ifc_req;
    };
}; // struct ifconf

struct ifreq 
{
#define IFNAMSIZ 16
    char ifr_name[IFNAMSIZ];
    union 
    {
        struct sockaddr ifru_addr;
        struct sockaddr ifru_dstaddr;
        struct sockaddr ifru_broadaddr;
        struct sockaddr ifru_netmask;
        struct sockaddr ifru_hwaddr;
        short ifru_flags;
        int ifru_metric;
        int64_t ifru_vnetid;
        uint64_t ifru_media;
        void* ifru_data;
        unsigned int ifru_index;
    } ifr_ifru;

    #define ifr_addr ifr_ifru.ifru_addr           
    #define ifr_dstaddr ifr_ifru.ifru_dstaddr     
    #define ifr_broadaddr ifr_ifru.ifru_broadaddr 
    #define ifr_netmask ifr_ifru.ifru_netmask     
    #define ifr_flags ifr_ifru.ifru_flags         
    #define ifr_metric ifr_ifru.ifru_metric       
    #define ifr_mtu ifr_ifru.ifru_metric          
    #define ifr_hardmtu ifr_ifru.ifru_metric      
    #define ifr_media ifr_ifru.ifru_media         
    #define ifr_rdomainid ifr_ifru.ifru_metric    
    #define ifr_vnetid ifr_ifru.ifru_vnetid       
    #define ifr_ttl ifr_ifru.ifru_metric          
    #define ifr_data ifr_ifru.ifru_data           
    #define ifr_index ifr_ifru.ifru_index        
    #define ifr_llprio ifr_ifru.ifru_metric      
    #define ifr_hwaddr ifr_ifru.ifru_hwaddr       
}; // struct ifreq 

struct if_nameindex 
{
    unsigned int if_index;
    char* if_name;
}; // struct if_nameindex

#ifdef __cplusplus
}
#endif
