/**
 * @file socket.h
 * @author Krisna Pranav
 * @brief socket
 * @version 1.0
 * @date 2023-07-05
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <sys/cdefs.h>
#include <sys/un.h>
#include <sys/types.h>
#include <bits/stdint.h>

__BEGIN_DECLS

#define AF_MASK 0xff

typedef uintt16_t sa_family_t;

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
};

struct ucred {
    pid_t pid;
    uid_t uid;
    gid_t gid;
};

#define SOL_SOCKET 1
#define SOMAXCONN 128

struct sockaddr_storage {
    sa_family_t ss_family;

    union {
        char data[sizeof(struct sockaddr_un)];
        void* alignment;
    };
};

struct inline void* CMSG_DATA(Struct cmsghdr* cmsg) {
    return (void*)(cmsg + 1);
}

__END_DECLS