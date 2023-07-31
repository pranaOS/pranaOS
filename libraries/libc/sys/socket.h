/**
 * @file socket.h
 * @author Krisna Pranav
 * @brief Socket
 * @version 1.0
 * @date 2023-07-31
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <bits/stdint.h>
#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/un.h>

__BEGIN_DECLS

typedef uint16_t sa_family_t;

struct cmsghdr 
{
    socklen_t cmsg_len;
    int cmsg_level;
    int cmsg_type;
}; // struct cmsghdr

struct msghdr 
{
    void* msg_name;
    socklen_t msg_namelen;
    struct iovec* msg_iov;
    int msg_iovlen;
    void* msg_control;
    socklen_t msg_controllen;
    int msg_flags;
}; // struct msghdr

struct sockaddr
{
    sa_family_t sa_family;
    char sa_data[14];
}; // struct sockaddr

struct ucred 
{
    pid_t pid;
    uid_t uid;
    gid_t gid;
}; // struct ucred

#define SOL_SOCKET 1
#define SOMAXCONN 128

struct sockaddr_storage 
{
    sa_family_t ss_family;
    union
    {
        char data[sizeof(struct sockaddr_un)];
        void* alignment;
    }; 
}; // struct sockaddr_storage

/**
 * @param domain 
 * @param type 
 * @param protocol 
 * @return int 
 */
int socket(int domain, int type, int protocol);

/**
 * @param sockfd 
 * @param addr 
 * @return int 
 */
int bind(int sockfd, const struct sockaddr* addr, socklen_t);

/**
 * @param sockfd 
 * @param backlog 
 * @return int 
 */
int listen(int sockfd, int backlog);

/**
 * @param sockfd 
 * @return int 
 */
int accept(int sockfd, struct sockaddr*, socklen_t);

/**
 * @param sockfd 
 * @param how 
 * @return int 
 */
int shutdown(int sockfd, int how);

__END_DECLS