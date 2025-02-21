/**
 * @file socket.h
 * @author Krisna Pranav
 * @brief Socket
 * @version 6.0
 * @date 2023-07-31
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/api/posix/sys/socket.h>
#include <sys/un.h>

__BEGIN_DECLS

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
int accept(int sockfd, struct sockaddr*, socklen_t*);

/**
 * @param sockfd 
 * @return int 
 */
int accept4(int sockfd, struct sockaddr*, socklen_t*, int);

/**
 * @param sockfd 
 * @return int 
 */
int connect(int sockfd, const struct sockaddr*, socklen_t);

/**
 * @param sockfd 
 * @param how 
 * @return int 
 */
int shutdown(int sockfd, int how);

/**
 * @param sockfd 
 * @param flags 
 * @return ssize_t 
 */
ssize_t send(int sockfd, void const*, size_t, int flags);

/**

 * @param sockfd 
 * @param flags 
 * @return ssize_t 
 */
ssize_t sendmsg(int sockfd, const struct msghdr*, int flags);

/**
 * @param sockfd 
 * @param flags 
 * @return ssize_t 
 */
ssize_t sendto(int sockfd, void const*, size_t, int flags, const struct sockaddr*, socklen_t);

/**
 * @param sockfd 
 * @param flags 
 * @return ssize_t 
 */
ssize_t recv(int sockfd, void*, size_t, int flags);

/**
 * @param sockfd 
 * @param flags 
 * @return ssize_t 
 */
ssize_t recvmsg(int sockfd, struct msghdr*, int flags);

/**
 * @param sockfd 
 * @param flags 
 * @return ssize_t 
 */
ssize_t recvfrom(int sockfd, void*, size_t, int flags, struct sockaddr*, socklen_t*);

/**
 * @param sockfd 
 * @param level 
 * @param option 
 * @return int 
 */
int getsockopt(int sockfd, int level, int option, void*, socklen_t*);

/**
 * @param sockfd 
 * @param level 
 * @param option 
 * @return int 
 */
int setsockopt(int sockfd, int level, int option, void const*, socklen_t);

/**
 * @param sockfd 
 * @return int 
 */
int getsockname(int sockfd, struct sockaddr*, socklen_t*);

/**
 * @param sockfd 
 * @return int 
 */
int getpeername(int sockfd, struct sockaddr*, socklen_t*);

/**
 * @param domain 
 * @param type 
 * @param protocol 
 * @param sv 
 * @return int 
 */
int socketpair(int domain, int type, int protocol, int sv[2]);

/**
 * @param sockfd 
 * @param fd 
 * @return int 
 */
int sendfd(int sockfd, int fd);

/**
 * @param sockfd 
 * @param options 
 * @return int 
 */
int recvfd(int sockfd, int options);


#define CMSG_ALIGN(x) (((x) + sizeof(void*) - 1) & ~(sizeof(void*) - 1))
#define CMSG_SPACE(x) (CMSG_ALIGN(sizeof(struct cmsghdr)) + CMSG_ALIGN(x))
#define CMSG_LEN(x) (CMSG_ALIGN(sizeof(struct cmsghdr)) + (x))

/**
 * @param msg 
 * @return struct cmsghdr* 
 */
static inline struct cmsghdr* CMSG_FIRSTHDR(struct msghdr* msg)
{
    if (msg->msg_controllen < sizeof(struct cmsghdr))
        return 0;
    return (struct cmsghdr*)msg->msg_control;
}

/**
 * @param msg 
 * @param cmsg 
 * @return struct cmsghdr* 
 */
static inline struct cmsghdr* CMSG_NXTHDR(struct msghdr* msg, struct cmsghdr* cmsg)
{
    struct cmsghdr* next = (struct cmsghdr*)((char*)cmsg + CMSG_ALIGN(cmsg->cmsg_len));
    unsigned offset = (char*)next - (char*)msg->msg_control;
    if (msg->msg_controllen < offset + sizeof(struct cmsghdr))
        return NULL;
    return next;
}

/**
 * @param cmsg 
 * @return void* 
 */
static inline void* CMSG_DATA(struct cmsghdr* cmsg)
{
    return (void*)(cmsg + 1);
}

__END_DECLS
