#include <unistd.h>
#include <cerrno>
#include <libpranaos/syscalls.h>

ssize_t read(int fd, void* buff, size_t count)
{
    size_t read = 0;
    utils::result result = ackos::sys_wrapper::stream_read(fd, buff, count, &read);

    if(result == utils::result::ERR_INVALID_FD)
    {
        errno = EBADF;

        return -1;
    }
    else if(result == utils::result::ERR_NOT_READABLE || result == utils::result::ERR_UNIMPLEMENTED)
    {
        errno = EINVAL;

        return -1;
    }
    else if(result == utils::result::ERR_INVALID_ADDRESS)
    {
        errno = EFAULT;

        return -1;
    }
    else
    {
        errno = 0;
    }

    return (ssize_t)read;
}

ssize_t write(int fd, const void* buff, size_t count)
{
    size_t written = 0;
    utils::result result = ackos::sys_wrapper::stream_write(fd, buff, count, &written);

    if(result == utils::result::ERR_INVALID_FD)
    {
        errno = EBADF;
    }
    else
    {
        errno = 0;
    }

    return (ssize_t)written;
}