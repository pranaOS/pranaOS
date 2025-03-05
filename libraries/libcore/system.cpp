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
