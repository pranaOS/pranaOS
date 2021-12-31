// TODO: implement syscalls
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

    va_end(va);

    return res;
}