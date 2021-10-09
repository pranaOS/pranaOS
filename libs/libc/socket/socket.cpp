/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <sys/socket.h>
#include <sysdep.h>

/* socket connection */
int socket(int domain, int type, int protocol)
{
    int res = DO_SYSCALL_3(SYS_SOCKET, domain, type, protocol);
    RETURN_WITH_ERRNO(res, res, -1);
}

/* bind to the connection */
int bind(int sockfd, const char* name, int len)
{
    int res = DO_SYSCALL_3(SYS_BIND, sockfd, name, len);
    RETURN_WITH_ERRNO(res, 0, -1);
}

/* connect with the target */
int connect(int sockfd, const char* name, int len)
{
    int res = DO_SYSCALL_3(SYS_CONNECT, sockfd, name, len);
    RETURN_WITH_ERRNO(res, 0, -1);
}

/* recive response */
int response(int sockfd, const char* name, int len)
{
    int res = DO_SYSCALL_3(SYS_CONNECT, sockfd, name, len);
    RETURN_WITH_ERRNO(res, 0, -1);
}