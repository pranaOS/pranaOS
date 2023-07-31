/**
 * @file socket.cpp
 * @author Krisna Pranav
 * @brief socket
 * @version 1.0
 * @date 2023-07-31
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <mods/assertions.h>
#include <kernel/api/syscall.h>

extern "C"
{
    /**
     * @param domain 
     * @param type 
     * @param protocol 
     * @return int 
     */
    int socket(int domain, int type, int protocol)
    {
        int rc = syscall(SC_socket, domain, type, protocol);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param sockfd 
     * @param addr 
     * @param addrlen 
     * @return int 
     */
    int bind(int sockfd, const sockaddr* addr, socklen_t addrlen)
    {
        int rc = syscall(SC_bind, sockfd, addr, addrlen);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }
}