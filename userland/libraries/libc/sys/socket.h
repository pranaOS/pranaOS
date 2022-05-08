#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#ifndef __socklen_t_defined
typedef __socklen_t socklen_t;
#define __socklen_t_defined
#endif

#define AF_UNIX 1	 
#define AF_INET 2	 
#define AF_PACKET 17 

#define PF_UNIX AF_UNIX
#define PF_INET AF_INET
#define PF_PACKET AF_PACKET

#define SHUT_RD 0	
#define SHUT_WR 1	
#define SHUT_RDWR 2 

enum
{
	IPPROTO_IP = 0,	   
	IPPROTO_ICMP = 1,  
	IPPROTO_TCP = 6,   
	IPPROTO_UDP = 17,  
	IPPROTO_RAW = 255, 
	IPPROTO_MAX
};

typedef unsigned short sa_family_t;

struct sockaddr {
    sa_family_t sa_family;
    char sa_data[14];
};

struct sockaddr_in {
    uint16_t sin_port;
    uint32_t sin_addr;
};

#define PACKET_HOST 0	   
#define PACKET_BROADCAST 1 
#define PACKET_MULTICAST 2 
#define PACKET_OTHERHOST 3 
#define PACKET_OUTGOING 4  
#define PACKET_LOOPBACK 5  
#define PACKET_USER 6	   
#define PACKET_KERNEL 7	   
#define PACKET_FASTROUTE 6 

#endif 