/**
 * @file in.h
 * @author Krisna Pranav
 * @brief in
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

typedef uint32_t in_addr_t;

#define INADDR_ANY ((in_addr_t)0)
#define INADDR_NONE ((in_addr_t)-1)
#define INADDR_LOOPBACK 0x7f000001
#define INADDR_BROADCAST 0xffffffff

#define IN_LOOPBACKNET 127

#define IP_TOS 1
#define IP_TTL 2
#define IP_MULTICAST_LOOP 3
#define IP_ADD_MEMBERSHIP 4
#define IP_DROP_MEMBERSHIP 5
#define IP_MULTICAST_IF 6
#define IP_MULTICAST_TTL 7
#define IP_BLOCK_SOURCE 8
#define IP_UNBLOCK_SOURCE 9
#define IP_OPTIONS 10

#define IPTOS_LOWDELAY 16
#define IPTOS_THROUGHPUT 8
#define IPTOS_RELIABILITY 4

#define MCAST_JOIN_SOURCE_GROUP 100
#define MCAST_LEAVE_SOURCE_GROUP 101

#define IPPORT_RESERVED 1024
#define IPPORT_USERRESERVED 5000

typedef uint16_t in_port_t;

struct in_addr 
{
    uint32_t s_addr;
}; // struct in_addr

struct sockaddr_in 
{
    sa_family_t sin_family;
    in_port_t sin_port;
    struct in_addr sin_addr;
    char sin_zero[8];
}; // struct sockaddr_in

struct ip_mreq 
{
    struct in_addr imr_multiaddr;
    struct in_addr imr_interface;
}; // struct ip_mreq

struct group_source_req 
{
    uint32_t gsr_interface;
    struct sockaddr_storage gsr_group;
    struct sockaddr_storage gsr_source;
}; // struct group_source_req

struct ip_mreq_source 
{
    struct in_addr imr_multiaddr;
    struct in_addr imr_sourceaddr;
    struct in_addr imr_interface;
}; // struct ip_mreq_source

#define IPV6_UNICAST_HOPS 1
#define IPV6_MULTICAST_HOPS 2
#define IPV6_MULTICAST_LOOP 3
#define IPV6_MULTICAST_IF 4
#define IPV6_ADD_MEMBERSHIP 5
#define IPV6_DROP_MEMBERSHIP 6
#define IP_ADD_SOURCE_MEMBERSHIP 7
#define IP_DROP_SOURCE_MEMBERSHIP 8
#define IPV6_V6ONLY 9
#define IPV6_JOIN_GROUP 5
#define IPV6_LEAVE_GROUP 6

struct in6_addr 
{
    uint8_t s6_addr[16];
}; // struct in6_addr

#define IN6ADDR_ANY_INIT                               \
    {                                                  \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 \
    }

#define IN6ADDR_LOOPBACK_INIT                          \
    {                                                  \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 \
    }

extern const struct in6_addr in6addr_any;
extern const struct in6_addr in6addr_loopback;

struct sockaddr_in6 
{
    sa_family_t sin6_family;   
    in_port_t sin6_port;       
    uint32_t sin6_flowinfo;    
    struct in6_addr sin6_addr; 
    uint32_t sin6_scope_id;    
}; // struct sockaddr_in6

struct ipv6_mreq 
{
    struct in6_addr ipv6mr_multiaddr;
    uint32_t ipv6mr_interface;
}; // struct ipv6_mreq

#ifdef __cplusplus
}
#endif
