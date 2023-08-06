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

struct ifreq {
#define IFNAMSIZ 16
    char ifr_name[IFNAMSIZ];
    union {
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
};

__END_DECLS
