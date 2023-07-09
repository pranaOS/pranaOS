/**
 * @file syscall.h
 * @author Krisna Pranav
 * @brief syscall
 * @version 1.0
 * @date 2023-07-09
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>
#include <mods/usrspace.h>

#ifdef __pranaos__
#   include <libraries/libc/fd_set.h>
#endif

extern "C" {
    struct pollfd;
    struct timeval;
    struct timespec;
    struct sockaddr;
    struct siginfo;
    struct stat;
    typedef u32 sockln_t;
}

namespace Kernel {

    namespace Syscall {
        enum Function {
            #undef __ENUMERATE_SYSCALL
            #define __ENUMERATE_SYSCALL(x) SC_##x,
            #undef
        }
    }

}