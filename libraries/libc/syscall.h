#pragma once

/**
 * this contains the syscalls require for the kernel or proejct
 * @brief this namespace contains all the syscall functions 
 * 
 */

namespace LibC {
    #define SYSCALL_RET_SUCCES 1
    #define SYSCALL_RET_ERROR 0
    #define PROC_ARG_LEN_MAX 100

    enum Syscalls {
        SYSCALL_EXIT = 0, 
        SYSCALL_LOG,
        SYSCALL_PRINT,
        SYSCALL_FILE_EXISTS,
        SYSCALL_DIR_EXISTS,
        SYSCALL_GET_FILESIZE,
        SYSCALL_READ_FILE,
        SYSCALL_WRITE_FILE,
        SYSCALL_CREATE_FILE,
        SYSCALL_CREATE_DIRECTORY,
        SYSCALL_EJECT_DISK,
        SYSCALL_GUI_GETLFB,
        SYSCALL_GET_SCREEN_PROPERTIES,
        SYSCALL_GET_HEAP_START,
        SYSCALL_GET_HEAP_END,
        SYSCALL_SET_HEAP_SIZE,
        SYSCALL_CREATE_SHARED_MEM,
        SYSCALL_REMOVE_SHARED_MEM,
        SYSCALL_MAP_SYSINFO,
        SYSCALL_RUN_PROC,
        SYSCALL_SLEEP_MS,
        SYSCALL_START_THREAD,
        SYSCALL_YIELD,
        SYSCALL_PROC_EXIST,
        SYSCALL_UNBLOCK,
        SYSCALL_SET_SCHEDULER,
        SYSCALL_GET_ARGUMENTS,
        SYSCALL_IPC_SEND,
        SYSCALL_IPC_RECEIVE,
        SYSCALL_IPC_AVAILABLE,
        SYSCALL_GET_TICKS,
        SYSCALL_GET_DATETIME,
        SYSCALL_SHUTDOWN,
        SYSCALL_REBOOT,
        SYSCALL_READ_STDIO,
        SYSCALL_WRITE_STDIO,
        SYSCALL_REDIRECT_STDIO,
        SYSCALL_STDIO_AVAILABLE,
        SYSCALL_BEGIN_LISTING,
        SYSCALL_LISTING_ENTRY,
        SYSCALL_END_LISTING,
        SYSCALL_GET_SYSINFO_VALUE,
    };

    int doSyscall(unsigned int intNum, unsigned int arg1 = 0, unsigned int arg2 = 0, unsigned int arg3 = 0, unsigned int arg4 = 0, unsigned int arg5 = 0);
}