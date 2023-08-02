/**
 * @file netdb.h
 * @author Krisna Pranav
 * @brief netdb
 * @version 1.0
 * @date 2023-08-02
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

struct hostent 
{
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
struct hostent* gethostbyname(const char*);

/**
 * @param addr 
 * @param len 
 * @param type 
 * @return struct hostent* 
 */
struct hostent* gethostbyaddr(const void* addr, socklen_t len, int type);

struct servent 
{
    char* s_name;
    char** s_aliases;
    int s_port;
    char* s_proto;
}; // struct servent

/**
 * @return struct servent* 
 */
struct servent* getservent();

/**
 * @param name 
 * @param protocol 
 * @return struct servent* 
 */
struct servent* getservbyname(const char* name, const char* protocol);

/**
 * @param port 
 * @param protocol 
 * @return struct servent* 
 */
struct servent* getservbyport(int port, const char* protocol);

/**
 * @param stay_open 
 */
void setservent(int stay_open);

void endservent();

struct protoent 
{
    char* p_name;
    char** p_aliases;
    int p_proto;
}; // struct protoent

void endprotoent();

/**
 * @param name 
 * @return struct protoent* 
 */
struct protoent* getprotobyname(const char* name);

/**
 * @param proto 
 * @return struct protoent* 
 */
struct protoent* getprotobynumber(int proto);

/**
 * @return struct protoent* 
 */
struct protoent* getprotoent();

/**
 * @param stay_open 
 */
void setprotoent(int stay_open);

extern int h_errno;

/// @brief: NOT_FOUND CODES
#define HOST_NOT_FOUND 101
#define NO_DATA 102
#define NO_RECOVERY 103
#define TRY_AGAIN 104

__END_DECLS
