#pragma once

#include <libpranaos/utils/result.h>
#include <libpranaos/system_info.h>
#include <cstdarg>
#include <cstddef>

namespace pranaos {
    enum {
        SYSCALL_STREAM_READ,
        SYSCALL_STREAM_WRITE,
        SYSCALL_STREAM_OPEN,
        SYSCALL_STREAM_CLOSE,
        SYSCALL_STREAM_IOCALL,
        SYSCALL_STREAM_CLONE1,
        SYSCALL_STREAM_CLONE2,
        SYSCALL_STREAM_GET_INFO
    };

    utils::result syscall_dispatch(int call, void* arg1, void* arg2, void* arg3, void* arg4, void* arg5, void* arg6);

    namespace sys_wrapper {
        utils::result stream_read(int fd, void* buf, size_t size, size_t* bytes_read);
    }
}