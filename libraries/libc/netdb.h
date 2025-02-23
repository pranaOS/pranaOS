/**
 * @file netdb.h
 * @author Krisna Pranav
 * @brief netdb
 * @version 6.0
 * @date 2023-08-02
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

struct hostent {
    char* h_name;
    char** h_aliases;
    int h_addrtype;
    int h_length;
    char** h_addr_list;
#define h_addr h_addr_list[0]
}; // struct hostent

/**
 * @return struct hostent* 
 */
struct hostent* gethostbyname(char const*);

/**
 * @param addr 
 * @param len 
 * @param type 
 * @return struct hostent* 
 */
struct hostent* gethostbyaddr(void const* addr, socklen_t len, int type);

struct servent {
    char* s_name;
    char** s_aliases;
    int s_port;
    char* s_proto;
}; // struct servent

/**
 * @return struct servent* 
 */
struct servent* getservent(void);

/**
 * @param name 
 * @param protocol 
 * @return struct servent* 
 */
struct servent* getservbyname(char const* name, char const* protocol);

/**
 * @param port 
 * @param protocol 
 * @return struct servent* 
 */
struct servent* getservbyport(int port, char const* protocol);

/**
 * @param stay_open 
 */
void setservent(int stay_open);
void endservent(void);

struct protoent {
    char* p_name;
    char** p_aliases;
    int p_proto;
}; // struct protoent

void endprotoent(void);

/**
 * @param name 
 * @return struct protoent* 
 */
struct protoent* getprotobyname(char const* name);

/**
 * @param proto 
 * @return struct protoent* 
 */
struct protoent* getprotobynumber(int proto);

/**
 * @return struct protoent* 
 */
struct protoent* getprotoent(void);

/**
 * @param stay_open 
 */
void setprotoent(int stay_open);

extern int h_errno;

#define HOST_NOT_FOUND 101
#define NO_DATA 102
#define NO_RECOVERY 103
#define TRY_AGAIN 104

struct addrinfo {
    int ai_flags;
    int ai_family;
    int ai_socktype;
    int ai_protocol;
    socklen_t ai_addrlen;
    struct sockaddr* ai_addr;
    char* ai_canonname;
    struct addrinfo* ai_next;
}; // struct addinfo

#define EAI_ADDRFAMILY 1
#define EAI_AGAIN 2
#define EAI_BADFLAGS 3
#define EAI_FAIL 4
#define EAI_FAMILY 5
#define EAI_MEMORY 6
#define EAI_NODATA 7
#define EAI_NONAME 8
#define EAI_SERVICE 9
#define EAI_SOCKTYPE 10
#define EAI_SYSTEM 11
#define EAI_OVERFLOW 12

#define AI_PASSIVE 0x0001
#define AI_CANONNAME 0x0002
#define AI_NUMERICHOST 0x0004
#define AI_NUMERICSERV 0x0008
#define AI_V4MAPPED 0x0010
#define AI_ALL 0x0020
#define AI_ADDRCONFIG 0x0040

#define NI_MAXHOST 1025
#define NI_MAXSERV 32

#define NI_NUMERICHOST (1 << 0)
#define NI_NUMERICSERV (1 << 1)
#define NI_NAMEREQD (1 << 2)
#define NI_NOFQDN (1 << 3)
#define NI_DGRAM (1 << 4)

/**
 * @param node 
 * @param service 
 * @param hints 
 * @param res 
 * @return int 
 */
int getaddrinfo(char const* __restrict node, char const* __restrict service, const struct addrinfo* __restrict hints, struct addrinfo** __restrict res);

/**
 * @param res 
 */
void freeaddrinfo(struct addrinfo* res);

/**
 * @param errcode 
 * @return char const* 
 */
char const* gai_strerror(int errcode);

/**
 * @param addr 
 * @param addrlen 
 * @param host 
 * @param hostlen 
 * @param serv 
 * @param servlen 
 * @param flags 
 * @return int 
 */
int getnameinfo(const struct sockaddr* __restrict addr, socklen_t addrlen, char* __restrict host, socklen_t hostlen, char* __restrict serv, socklen_t servlen, int flags);

__END_DECLS
