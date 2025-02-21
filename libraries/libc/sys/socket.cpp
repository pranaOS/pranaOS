/**
 * @file socket.cpp
 * @author Krisna Pranav
 * @brief socket
 * @version 6.0
 * @date 2023-07-31
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/assertions.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <syscall.h>

extern "C" {

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
    int bind(int sockfd, sockaddr const* addr, socklen_t addrlen)
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
        return accept4(sockfd, addr, addrlen, 0);
    }

    /**
     * @param sockfd 
     * @param addr 
     * @param addrlen 
     * @param flags 
     * @return int 
     */
    int accept4(int sockfd, sockaddr* addr, socklen_t* addrlen, int flags)
    {
        Syscall::SC_accept4_params params { addr, addrlen, sockfd, flags };
        int rc = syscall(SC_accept4, &params);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param sockfd 
     * @param addr 
     * @param addrlen 
     * @return int 
     */
    int connect(int sockfd, sockaddr const* addr, socklen_t addrlen)
    {
        int rc = syscall(SC_connect, sockfd, addr, addrlen);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param sockfd 
     * @param how 
     * @return int 
     */
    int shutdown(int sockfd, int how)
    {
        int rc = syscall(SC_shutdown, sockfd, how);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param sockfd 
     * @param msg 
     * @param flags 
     * @return ssize_t 
     */
    ssize_t sendmsg(int sockfd, const struct msghdr* msg, int flags)
    {
        int rc = syscall(SC_sendmsg, sockfd, msg, flags);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param sockfd 
     * @param data 
     * @param data_length 
     * @param flags 
     * @param addr 
     * @param addr_length 
     * @return ssize_t 
     */
    ssize_t sendto(int sockfd, void const* data, size_t data_length, int flags, const struct sockaddr* addr, socklen_t addr_length)
    {
        iovec iov = { const_cast<void*>(data), data_length };
        msghdr msg = { const_cast<struct sockaddr*>(addr), addr_length, &iov, 1, nullptr, 0, 0 };
        return sendmsg(sockfd, &msg, flags);
    }

    /**
     * @param sockfd 
     * @param data 
     * @param data_length 
     * @param flags 
     * @return ssize_t 
     */
    ssize_t send(int sockfd, void const* data, size_t data_length, int flags)
    {
        return sendto(sockfd, data, data_length, flags, nullptr, 0);
    }

    /**
     * @param sockfd 
     * @param msg 
     * @param flags 
     * @return ssize_t 
     */
    ssize_t recvmsg(int sockfd, struct msghdr* msg, int flags)
    {
        int rc = syscall(SC_recvmsg, sockfd, msg, flags);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param sockfd 
     * @param buffer 
     * @param buffer_length 
     * @param flags 
     * @param addr 
     * @param addr_length 
     * @return ssize_t 
     */
    ssize_t recvfrom(int sockfd, void* buffer, size_t buffer_length, int flags, struct sockaddr* addr, socklen_t* addr_length)
    {
        if (!addr_length && addr) {
            errno = EINVAL;
            return -1;
        }

        sockaddr_storage internal_addr;
        iovec iov = { buffer, buffer_length };
        msghdr msg = { addr ? &internal_addr : nullptr, addr ? (socklen_t)sizeof(internal_addr) : 0, &iov, 1, nullptr, 0, 0 };
        ssize_t rc = recvmsg(sockfd, &msg, flags);
        if (rc >= 0 && addr) {
            memcpy(addr, &internal_addr, min(*addr_length, msg.msg_namelen));
            *addr_length = msg.msg_namelen;
        }
        return rc;
    }

    /**
     * @param sockfd 
     * @param buffer 
     * @param buffer_length 
     * @param flags 
     * @return ssize_t 
     */
    ssize_t recv(int sockfd, void* buffer, size_t buffer_length, int flags)
    {
        return recvfrom(sockfd, buffer, buffer_length, flags, nullptr, nullptr);
    }

    /**
     * @param sockfd 
     * @param level 
     * @param option 
     * @param value 
     * @param value_size 
     * @return int 
     */
    int getsockopt(int sockfd, int level, int option, void* value, socklen_t* value_size)
    {
        Syscall::SC_getsockopt_params params { sockfd, level, option, value, value_size };
        int rc = syscall(SC_getsockopt, &params);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param sockfd 
     * @param level 
     * @param option 
     * @param value 
     * @param value_size 
     * @return int 
     */
    int setsockopt(int sockfd, int level, int option, void const* value, socklen_t value_size)
    {
        Syscall::SC_setsockopt_params params { value, sockfd, level, option, value_size };
        int rc = syscall(SC_setsockopt, &params);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param sockfd 
     * @param addr 
     * @param addrlen 
     * @return int 
     */
    int getsockname(int sockfd, struct sockaddr* addr, socklen_t* addrlen)
    {
        Syscall::SC_getsockname_params params { sockfd, addr, addrlen };
        int rc = syscall(SC_getsockname, &params);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param sockfd 
     * @param addr 
     * @param addrlen 
     * @return int 
     */
    int getpeername(int sockfd, struct sockaddr* addr, socklen_t* addrlen)
    {
        Syscall::SC_getpeername_params params { sockfd, addr, addrlen };
        int rc = syscall(SC_getpeername, &params);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param domain 
     * @param type 
     * @param protocol 
     * @param sv 
     * @return int 
     */
    int socketpair(int domain, int type, int protocol, int sv[2])
    {
        Syscall::SC_socketpair_params params { domain, type, protocol, sv };
        int rc = syscall(SC_socketpair, &params);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param sockfd 
     * @param fd 
     * @return int 
     */
    int sendfd(int sockfd, int fd)
    {
        int rc = syscall(SC_sendfd, sockfd, fd);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param sockfd 
     * @param options 
     * @return int 
     */
    int recvfd(int sockfd, int options)
    {
        int rc = syscall(SC_recvfd, sockfd, options);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

}
