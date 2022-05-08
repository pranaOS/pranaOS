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


#endif 