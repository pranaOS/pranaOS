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
    
    /**
     * @param sockfd 
     * @param backlog 
     * @return int 
     */
    int listen(int sockfd, int backlog)
    {
        int rc = syscall(SC_listen, sockfd, backlog);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param sockfd 
     * @param addr 
     * @param addrlen 
     * @return int 
     */
    int accept(int sockfd, sockaddr* addr, socklen_t* addrlen)
    {
        int rc = syscall(SC_accept, sockfd, addr, addrlen);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    } // int accept

    /**
     * @param sockfd 
     * @param addr 
     * @param addrlen 
     * @return int 
     */
    int connect(int sockfd, const sockaddr* addr, socklen_t addrlen)
    {
        int rc = syscall(SC_connect, sockfd, addr, addrlen);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    } // int connect

    /**
     * @param sockfd 
     * @param how 
     * @return int 
     */
    int shutdown(int sockfd, int how)
    {
        int rc = syscall(SC_shutdown, sockfd, how);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    } // int shutdown
}