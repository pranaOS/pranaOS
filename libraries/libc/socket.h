/**
 * @file socket.h
 * @author Krisna Pranav
 * @brief socket
 * @version 6.0
 * @date 2023-07-05
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <sys/cdefs.h>
#include <sys/un.h>
#include <sys/types.h>
#include <bits/stdint.h>

__BEGIN_DECLS

#define AF_MASK 0xff
#define AF_UNSPEC 0
#define AF_LOCAL 1
#define AF_UNIX AF_LOCAL
#define AF_INET 2
#define AF_MAX 3
#define PF_LOCAL AF_LOCAL
#define PF_UNIX PF_LOCAL
#define PF_INET AF_INET
#define PF_UNSPEC AF_UNSPEC
#define PF_MAX AF_MAX

#define SOCK_TYPE_MASK 0xff
#define SOCK_STREAM 1
#define SOCK_DGRAM 2
#define SOCK_RAW 3
#define SOCK_NONBLOCK 04000
#define SOCK_CLOEXEC 02000000

#define SHUT_RD 1
#define SHUT_WR 2
#define SHUT_RDWR 3

#define IPPROTO_IP 0
#define IPPROTO_ICMP 1
#define IPPROTO_TCP 6
#define IPPROTO_UDP 17

#define MSG_TRUNC 0x1
#define MSG_CTRUNC 0x2
#define MSG_DONTWAIT 0x40

typedef uint16_t sa_family_t;

struct cmsghdr {
    socklen_t cmsg_len;
    int cmsg_level;
    int cmsg_type;
};

struct msghdr {
    void* msg_name;
    socklen_t msg_namelen;
    struct iovec* msg_iov;
    int msg_iovlen;
    void* msg_control;
    socklen_t msg_controllen;
    int msg_flags;
};

struct sockaddr {
    sa_family_t sa_family;
    char sa_data[14];
};

struct ucred {
    pid_t pid;
    uid_t uid;
    gid_t gid;
};

#define SOL_SOCKET 1
#define SOMAXCONN 128

enum {
    SO_RCVTIMEO,
    SO_SNDTIMEO,
    SO_TYPE,
    SO_ERROR,
    SO_PEERCRED,
    SO_REUSEADDR,
    SO_BINDTODEVICE,
    SO_KEEPALIVE,
    SO_TIMESTAMP,
    SO_BROADCAST,
};

#define SO_RCVTIMEO SO_RCVTIMEO
#define SO_SNDTIMEO SO_SNDTIMEO
#define SO_TYPE SO_TYPE
#define SO_ERROR SO_ERROR
#define SO_PEERCRED SO_PEERCRED
#define SO_REUSEADDR SO_REUSEADDR
#define SO_BINDTODEVICE SO_BINDTODEVICE
#define SO_KEEPALIVE SO_KEEPALIVE
#define SO_TIMESTAMP SO_TIMESTAMP
#define SO_BROADCAST SO_BROADCAST

enum {
    SCM_TIMESTAMP,
    SCM_RIGHTS,
};

// SCM[TIMESTAMP, RIGHTS]
#define SCM_TIMESTAMP SCM_TIMESTAMP
#define SCM_RIGHTS SCM_RIGHTS

struct sockaddr_storage {
    sa_family_t ss_family;
    union {
        char data[sizeof(struct sockaddr_un)];
        void* alignment;
    };
};

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
ssize_t send(int sockfd, const void*, size_t, int flags);

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
ssize_t sendto(int sockfd, const void*, size_t, int flags, const struct sockaddr*, socklen_t);

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
int setsockopt(int sockfd, int level, int option, const void*, socklen_t);

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
 * @param sockfd 
 * @param fd 
 * @return int 
 */
int sendfd(int sockfd, int fd);

/**
 * @param sockfd 
 * @return int 
 */
int recvfd(int sockfd);

#define CMSG_ALIGN(x) (((x) + sizeof(void*) - 1) & ~(sizeof(void*) - 1))
#define CMSG_SPACE(x) (CMSG_ALIGN(sizeof(struct cmsghdr)) + CMSG_ALIGN(x))
#define CMSG_LEN(x) (CMSG_ALIGN(sizeof(struct cmsghdr)) + (x))

/**
 * @param msg 
 * @return struct cmsghdr* 
 */
static inline struct cmsghdr* CMSG_FIRSTHDR(struct msghdr* msg) {
    if (msg->msg_controllen < sizeof(struct cmsghdr))
        return 0;
    return (struct cmsghdr*)msg->msg_control;
}

/**
 * @param msg 
 * @param cmsg 
 * @return struct cmsghdr* 
 */
static inline struct cmsghdr* CMSG_NXTHDR(struct msghdr* msg, struct cmsghdr* cmsg) {
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
static inline void* CMSG_DATA(struct cmsghdr* cmsg) {
    return (void*)(cmsg + 1);
}

__END_DECLS
