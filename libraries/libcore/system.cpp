/**
 * @file system.cpp
 * @author Krisna Pranav
 * @brief system
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/fixedarray.h>
#include <mods/scopedvaluerollback.h>
#include <mods/stdlibextra.h>
#include <mods/string.h>
#include <mods/vector.h>
#include <libcore/system.h>
#include <libsystem/syscall.h>
#include <limits.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/ptrace.h>
#include <sys/time.h>
#include <termios.h>
#include <unistd.h>

#ifdef __pranaos__
#    include <pranaos.h>
#endif

#if defined(__linux__) && !defined(MFD_CLOEXEC)
#    include <linux/memfd.h>
#    include <sys/syscall.h>

/**
 * @param name 
 * @param flags 
 * @return int 
 */
static int memfd_create(char const* name, unsigned int flags)
{
    return syscall(SYS_memfd_create, name, flags);
}
#endif

#if defined(__APPLE__)
#    include <sys/mman.h>
#endif

#define HANDLE_SYSCALL_RETURN_VALUE(syscall_name, rc, success_value) \
    if ((rc) < 0) {                                                  \
        return Error::from_syscall(syscall_name, rc);                \
    }                                                                \
    return success_value;

namespace Core::System
{

#ifndef HOST_NAME_MAX
#   ifdef __APPLE__
#       define HOST_NAME_MAX 255
#   else
#       define HOST_NAME_MAX 64
#   endif
#endif

#ifdef __pranaos

/**
 * @return ErrorOr<void> 
 */
ErrorOr<void> beep()
{
    auto rc = ::sysbeep();

    if (rc < 0) {
        return Error::from_syscall("beep"sv, -errno);
    }

    return {};
}

/**
 * @param promises 
 * @param execpromises 
 * @return ErrorOr<void> 
 */
ErrorOr<void> pledge(StringView promises, StringView execpromises)
{
    Syscall::SC_pledge_params params {
        { promises.characters_without_null_termination(), promises.length() },
        { execpromises.characters_without_null_termination(), execpromises.length() },
    };
    int rc = syscall(SC_pledge, &params);
    HANDLE_SYSCALL_RETURN_VALUE("pledge"sv, rc, {});
}

/**
 * @param path 
 * @param permissions 
 * @return ErrorOr<void> 
 */
ErrorOr<void> unveil(StringView path, StringView permissions)
{
    Syscall::SC_unveil_params params {
        { path.characters_without_null_termination(), path.length() },
        { permissions.characters_without_null_termination(), permissions.length() },
    };
    int rc = syscall(SC_unveil, &params);
    HANDLE_SYSCALL_RETURN_VALUE("unveil"sv, rc, {});
}

/**
 * @param sockfd 
 * @param fd 
 * @return ErrorOr<void> 
 */
ErrorOr<void> sendfd(int sockfd, int fd)
{
    if (::sendfd(sockfd, fd) < 0)
        return Error::from_syscall("sendfd"sv, -errno);
    return {};
}

/**
 * @param sockfd 
 * @param options 
 * @return ErrorOr<int> 
 */
ErrorOr<int> recvfd(int sockfd, int options)
{
    auto fd = ::recvfd(sockfd, options);
    if (fd < 0)
        return Error::from_syscall("recvfd"sv, -errno);
    return fd;
}

#endif

} // namespace Core::System