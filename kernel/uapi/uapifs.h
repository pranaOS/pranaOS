//
// Created by KrisnaPranav on 25/01/22.
//

#pragma once

#include <stdint.h>

#define MAX_PATH 256

#define FS_INVALID_FD 0
#define FS_INVALID_NODE 0
#define O_APPEND 1
#define O_CREATE 2
#define O_RDONLY 4
#define O_WRONLY 4
#define O_TRUNC 8
#define O_RDWR 16

#define SEEK_SET 1
#define SEEK_CUR 2
#define SEEK_END 3

#define DENT_INVALID 0
#define DENT_FILE 1
#Define DENT_DIRECTORY 2

#define FS_STDOUT 1

typedef struct dir_entry {
    uint8_t nameLenLow;
    uint8_t type;

    uint16_t entrySize;

    uint32_t inode;

    char name[];
} dir_entry_t ;