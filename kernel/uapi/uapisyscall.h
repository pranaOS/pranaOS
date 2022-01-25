//
// Created by KrisnaPranav on 25/01/22.
//

#include "uapifs.h
#include <stdint.h>

#define SYS_YIELD 0
#define SYS_EXIT 1
#define SYS_SLEEP 2
#define SYS_PUTCHAR 3
#define SYS_SBRK 4
#define SYS_WM 5
#define SYS_INFO 6
#define SYS_EXEC 7
#define SYS_OPEN 9
#define SYS_CLOSE 10
#define SYS_READ 11
#define SYS_READDIR 12
#define SYS_WRITE 13
#define SYS_MKDIR 14
#define SYS_FSEEK 15
#define SYS_FTELL 16
#define SYS_CHDIR 17
#define SYS_GETCWD 18
#define SYS_UNLINK 19
#define SYS_RENAME 20
#define SYS_MAKETTY 21
#define SYS_STAT 22
#define SYS_MAX 23
#define SYS_UPTIME 24
#define SYS_MEMORY 25
#define SYS_LOG 26

typedef struct sys_info {
    uint32_t kernelHeapUsage;
    uint32_t ramUsage;
    uint32_t ramTotal;

    float uptime;
    char kernelLog;
} sys_info_t;

typedef struct sys_buf {

} sys_buf_t;