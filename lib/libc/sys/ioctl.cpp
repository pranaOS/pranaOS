#include <sys/ioctl.h>
#include <cstdarg>
#include <cerrno>
#include <libpranaos/syscalls.h>

int ioctl(int fd, int request, ...)
{
    va_list va;

    va_start(va, request);
    int res = 0;
    void* arg = va_arg(va, void*);

    auto result = pranaos::sys_wrapper::stream_iocall(fd, request, arg);

    switch((int)result)
    {
    case utils::result::ERR_INVALID_FD:
    {
        res = EBADF;

        break;
    }
    case utils::result::ERR_INVALID_ADDRESS:
    {
        res = EFAULT;

        break;
    }
    case utils::result::ERR_INVALID_ARGUMENT:
    {
        res = EINVAL;

        break;
    }
    default:
        break;
    }

    va_end(va);

    return res;
}