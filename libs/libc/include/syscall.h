#pragma once

namespace Syscalls {
    #define SYSCALL_RET_SUCCESS 1
    #define SYSCALL_RET_ERROR 0
    #define PROC_ARG_LEN_MAX 100

    enum SystCalls {
        SYSCALL_EXIT = 0,
        SYSCALL_LOG,
        SYSCALL_PRINT,
        SYSCALL_FILE_EXISTS,
        SYSCALL_DIR_EXISTS,
        SYSCALL_GET_FILESIZE,
        SYSCALL_READ_FILE,
        
    }
}