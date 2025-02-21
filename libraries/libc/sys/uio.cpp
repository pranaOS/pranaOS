/**
 * @file uio.cpp
 * @author Krisna Pranav
 * @brief UIO 
 * @version 6.0
 * @date 2023-07-31
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <errno.h>
#include <sys/uio.h>
#include <syscall.h>

extern "C" {

/**
 * @param fd 
 * @param iov 
 * @param iov_count 
 * @return ssize_t 
 */
ssize_t writev(int fd, const struct iovec* iov, int iov_count)
{
    int rc = syscall(SC_writev, fd, iov, iov_count);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}


/**
 * @param fd 
 * @param iov 
 * @param iov_count 
 * @return ssize_t 
 */
ssize_t readv(int fd, const struct iovec* iov, int iov_count)
{
    int rc = syscall(SC_readv, fd, iov, iov_count);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

} // extern "C"
